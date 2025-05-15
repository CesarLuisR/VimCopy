#pragma once

#include <vector>
#include "ConsoleUtils.h"
#include "SelectedText.h"

enum class Mode {
	Normal, Edit, Visual
};

class WindowsView {
private:
	int currentLine;
	int currentX;
	int currentY;
	int maxX;
	int startPoint;
	SelectedText selText;
	std::vector<std::string>& lines;

public:
	ConsoleSize cSize;
	Mode mode;

	WindowsView(std::vector<std::string>& lines, SelectedText selText);
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
	void HighlightOne();
	void RenderHighlight();
};
