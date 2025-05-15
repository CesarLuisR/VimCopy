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
	std::vector<SelectedPos> positions;
};

class SelectedText {
private:
	std::vector<SelectedLine> lines;
	int count;

public:
	SelectedText();
	std::vector<SelectedLine> GetLines();
	void AddLine(SelectedLine line);
	void RemoveLine();
	void AddPos(SelectedPos pos);
	void RemovePos();
	void Clear();
	int GetFirstPosX();
};