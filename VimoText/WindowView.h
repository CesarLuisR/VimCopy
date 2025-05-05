#pragma once

#include <vector>
#include "ConsoleUtils.h"

enum class Mode {
	Visual, Edit
};

class WindowsView {
private:
	int currentLine;
	int currentX;
	int currentY;
	int maxX;
	int startPoint;
	std::vector<std::string>& lines;

public:
	ConsoleSize cSize;
	Mode mode;

	WindowsView(std::vector<std::string>& lines);
	void IncreaseCurrentLine();
	void DecreaseCurrentLine();
	void UpdateLines(std::vector<std::string>& lines);
	void GoRight();
	void GoLeft();
	void XReAdjustment();
	void ChangeMode(const Mode mode);
	void VisualCommands();
	void GoTop();
	void GoBottom();
	void GoAllRight();
	void GoAllLeft();
	unsigned long int GetCurrentPos() const;
	void Render();
};
