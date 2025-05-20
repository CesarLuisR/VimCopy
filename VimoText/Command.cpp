#include "Command.h"

#include "Utils.h"
#include "WindowView.h"

void InsertTextCommand::execute() {
	_pt.InsertText(_text, _index);

    if (_formerLine == 0) {
		_formerX = _view.GetCurrentX();
		_formerY = _view.GetCurrentY();
		_formerLine = _view.GetCurrentLine();
		_formerStart = _view.GetCurrentStart();
    }

    if (_text == "\n") {
        auto lines = GetLines(_pt.GetText());
        _view.UpdateLines(lines);

        _view.IncreaseCurrentLine();
        _view.GoAllLeft();
    } else {
        // Esto me esta dando un problema
        _view.SetCurrentX(_view.GetCurrentX());
		_view.SetCurrentY(_formerY);
		_view.SetCurrentLine(_formerLine);
		_view.SetCurrentStart(_formerStart);
    }
}

void InsertTextCommand::undo() {
	_pt.RemoveText(_index, _text.size());

    _view.SetCurrentX(_formerX);
    _view.SetCurrentY(_formerY);
    _view.SetCurrentLine(_formerLine);
    _view.SetCurrentStart(_formerStart);
}

void RemoveTextCommand::execute() {
    if (_formerLine == 0) {
		_formerX = _view.GetCurrentX();
		_formerY = _view.GetCurrentY();
		_formerLine = _view.GetCurrentLine();
		_formerStart = _view.GetCurrentStart();
    }

	backup_text = _pt.GetText(_index, _length);
	_pt.RemoveText(_index, _length);

    _view.SetCurrentX(_formerX);
    _view.SetCurrentY(_formerY);
    _view.SetCurrentLine(_formerLine);
    _view.SetCurrentStart(_formerStart);
}

void RemoveTextCommand::undo() {
	_pt.InsertText(backup_text, _index);

    _view.SetCurrentX(_formerX);
    _view.SetCurrentY(_formerY);
    _view.SetCurrentLine(_formerLine);
    _view.SetCurrentStart(_formerStart);
}

void RemoveLineCommand::execute() {
    if (_formerLine == 0) {
		_formerX = _view.GetCurrentX();
		_formerY = _view.GetCurrentY();
		_formerLine = _view.GetCurrentLine();
		_formerStart = _view.GetCurrentStart();
    }

    int line = _view.GetCurrentLine() - 1;
    auto lines = _view.GetLines();
    int lineSize = lines[line].size();

	backup_text = _pt.GetText(_index, lineSize);
    CopyIntoClipboard(backup_text);
	_pt.RemoveText(_index, lineSize);

    _view.SetCurrentX(_formerX);
    _view.SetCurrentY(_formerY);
    _view.SetCurrentLine(_formerLine);
    _view.SetCurrentStart(_formerStart);
}

void RemoveLineCommand::undo() {
	_pt.InsertText(backup_text, _index);

    _view.SetCurrentX(_formerX);
    _view.SetCurrentY(_formerY);
    _view.SetCurrentLine(_formerLine);
    _view.SetCurrentStart(_formerStart);
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

