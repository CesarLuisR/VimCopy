#pragma once
#include <vector>

#include "ConsoleUtils.h"

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

	WindowsView(std::vector<std::string>& lines);
	void IncreaseCurrentLine();
	void DecreaseCurrentLine();
	void GoRight();
	void GoLeft();
	void XReAdjustment();
	void Render();
};
