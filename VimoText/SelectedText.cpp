#include "SelectedText.h"
#include "WindowView.h"

SelectedText::SelectedText(int linesCount) {
	selectedCount = 0;

	lines.reserve(linesCount);
	for (int i = 0; i < linesCount; i++) {
		SelectedLine emptyLine = { i, -1, -1, false, {} };
		lines.emplace_back(emptyLine);
	}
}

std::vector<SelectedLine> SelectedText::GetLines() {
	return lines;
}

void SelectedText::UpdateLines(int linesCount) {
	lines.clear();
	lines.reserve(linesCount);
	for (int i = 0; i < linesCount; i++) {
		SelectedLine emptyLine = { i, -1, -1, false, {} };
		lines.emplace_back(emptyLine);
	}
}

void SelectedText::AddLine(SelectedLine line) {
	lines[line.currentLine - 1] = line;
	lines[line.currentLine - 1].currentLine -= 1;
}

void SelectedText::RemoveLine(SelectedLine line) {
	//if (selectedCount == 0) return;

	selectedCount -= lines.back().positions.size();
	SelectedLine emptyLine = { line.currentLine - 1, -1, -1, false, {} };
	lines[line.currentLine - 1] = emptyLine;
}

void SelectedText::AddPos(SelectedPos pos) {
	lines[pos.currentLine - 1].positions.emplace_back(pos);
	lines[pos.currentLine - 1].endPos++;
	//selectedCount++;
}

void SelectedText::RemovePos(SelectedPos pos) {
	//if (selectedCount == 0) return;
	lines[pos.currentLine - 1].completed = false;
	lines[pos.currentLine - 1].positions.pop_back();
	lines[pos.currentLine - 1].endPos--;

	selectedCount--;
}

void SelectedText::Clear() {
	for (int i = 0; i < lines.size(); i++) {
		SelectedLine emptyLine = { i, -1, -1, false, {} };
		lines[i] = emptyLine;
	}
}

void SelectedText::AddFirstPos(int line) {
	firstPosLine = line - 1;
}

void SelectedText::AddScrollPos(int scrollPos) {
	scrollStartPos = scrollPos;
}

int SelectedText::GetScrollPos() {
	return scrollStartPos;
}

SelectedPos SelectedText::GetFirstPosX() {
	return lines[firstPosLine].positions[0];
}

SelectedPos SelectedText::GetLastPosX() {
	SelectedPos lastPos;

	for (SelectedLine line : lines) {
		if (line.startPos != -1) {
			lastPos = line.positions.back();
		}
	}
	
	return lastPos;
}


