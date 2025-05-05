#pragma once
#include <vector>

#include "PieceTable.h"
#include "WindowView.h"

struct Command {
	virtual ~Command() = default;
	virtual void execute() = 0;
	virtual void undo() = 0;
};

class InsertTextCommand : public Command {
private:
	PieceTable& _pt;
	int _index;
	std::string _text;
	WindowsView& _view;

public:
	InsertTextCommand(PieceTable& pt, int index, const std::string& text, WindowsView& view)
		: _pt(pt), _index(index), _text(text), _view(view) {}

	void execute() override;
	void undo() override;
};

class RemoveTextCommand : public Command {
private:
	PieceTable& _pt;
	int _index;
	int _length;
	std::string backup_text;
	WindowsView& _view;

public:
	RemoveTextCommand(PieceTable& pt, int index, int length, WindowsView& view)
		: _pt(pt), _index(index), _length(length), _view(view) {}

	void execute() override;
	void undo() override;
};

class CommandManager {
private:
    std::vector<std::unique_ptr<Command>> _undoStack, _redoStack;

public:
	void executeCommand(std::unique_ptr<Command> cmd);
	void undo();
	void redo();
};
