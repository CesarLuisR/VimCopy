#include "Command.h"
#include "WindowView.h"

void InsertTextCommand::execute() {
	_pt.InsertText(_text, _index);

    if (_text == "\n") {
        _view.IncreaseCurrentLine();
        _view.GoAllLeft();
    } else {
		_view.GoRight();
    }
}

void InsertTextCommand::undo() {
	_pt.RemoveText(_index, _text.size());

    if (_text == "\n") {
        _view.DecreaseCurrentLine();

    } else {
		_view.GoLeft();
    }
}

void RemoveTextCommand::execute() {
	backup_text = _pt.GetText(_index, _length);
	_pt.RemoveText(_index, _length);
}

void RemoveTextCommand::undo() {
	_pt.InsertText(backup_text, _index);
}

void CommandManager::executeCommand(std::unique_ptr<Command> cmd) {
	cmd->execute();
	_undoStack.push_back(std::move(cmd));
	_redoStack.clear();
}

void CommandManager::undo() {
    if (_undoStack.empty()) return;
    auto cmd = std::move(_undoStack.back());
    _undoStack.pop_back();
    cmd->undo();
    _redoStack.push_back(std::move(cmd));
}

void CommandManager::redo() {
    if (_redoStack.empty()) return;
    auto cmd = std::move(_redoStack.back());
    _redoStack.pop_back();
    cmd->execute();
    _undoStack.push_back(std::move(cmd));
}

