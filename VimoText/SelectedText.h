#pragma once
#include <vector>

struct SelectedPos {
	int currentLine;
	int currentX;
	int currentY;
	unsigned long long int curPos;
};

struct SelectedLine {
	int currentLine;
	int startPos;
	int endPos;
	bool completed;
	std::vector<SelectedPos> positions;
};

class SelectedText {
private:
	std::vector<SelectedLine> lines;
	int firstPosLine;
	int scrollStartPos;

public:
	SelectedText(int linesCount);
	void UpdateLines(int linesCount);
	std::vector<SelectedLine> GetLines();
	void AddLine(SelectedLine line);
	void RemoveLine(SelectedLine line);
	void AddPos(SelectedPos pos);
	void RemovePos(SelectedPos pos);
	void Clear();
	void AddFirstPos(int);
	void AddScrollPos(int);
	int GetScrollPos();
	SelectedPos GetFirstPosX();
};

