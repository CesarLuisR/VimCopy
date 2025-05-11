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
	const std::vector<std::string>& GetLines();
	bool GoRight();
	bool GoLeft();
	void XReAdjustment();
	void ChangeMode(const Mode mode);
	void VisualCommands(char c);
	void GoTop();
	void GoBottom();
	void GoAllRight();
	void GoAllLeft();
	int GetCurrentX();
	void SetCurrentX(int x);
	int GetCurrentY();
	void SetCurrentY(int);
	int GetCurrentLine();
	void SetCurrentLine(int);
	int GetCurrentStart();
	void SetCurrentStart(int);
	unsigned long int GetCurrentPos() const;
	void Render();
};
