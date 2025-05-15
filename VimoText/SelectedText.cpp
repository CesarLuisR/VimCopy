#include "SelectedText.h"
#include "WindowView.h"

SelectedText::SelectedText() {
	count = 0;
}

std::vector<SelectedLine> SelectedText::GetLines() {
	return lines;
}

void SelectedText::AddLine(SelectedLine line) {
	lines.emplace_back(line);
}

void SelectedText::RemoveLine() {
	if (count == 0) return;

	count -= lines.back().positions.size();
	if (lines.empty()) return;
	lines.pop_back();
}

void SelectedText::AddPos(SelectedPos pos) {
	lines.back().positions.emplace_back(pos);
	lines.back().endPos++;
	count++;
}

void SelectedText::RemovePos() {
	if (count == 0) return;

	if (lines.empty()) return;
	if (lines.back().positions.empty()) return;
	
	lines.back().positions.pop_back();
	lines.back().endPos--;

	count--;
}

void SelectedText::Clear() {
	lines.clear();
}

int SelectedText::GetFirstPosX() {
	return lines.begin()->startPos;
}

