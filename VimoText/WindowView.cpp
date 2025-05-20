#include "WindowView.h"

#include <fstream>
#include <iomanip>

// WindowsView: renders text lines in a console with line numbers and cursor navigation.
WindowsView::WindowsView(std::vector<std::string>& _lines, SelectedText& selText, PieceTable& pt, std::ofstream& file)
    : currentLine(1),    // 1-based index of the cursor line
    currentX(10),      // X position of the cursor (column)
    currentY(0),       // Y position of the cursor (row)
    maxX(10),
    startPoint(0),     // Index of first visible line in 'lines'
	mode(Mode::Normal),
    lines(_lines),      // Reference to the text buffer
    selText(selText),
	pt(pt),
	file(file)
{}

// Render the current view: clear screen, draw line numbers and content
void WindowsView::Render() {
    std::cout << "\033[?25l";  // Hide cursor
    cSize = GetConsoleSize();  // Update console dimensions

	auto selLines = selText.GetLines();

    // Draw each visible line (excluding bottom status row)
    for (int i = 0; i < cSize.height - 1; i++) {
        int idx = startPoint + i;
        if (idx >= lines.size())
            break;  // No more lines to display

        // Clear the line
        GoTo(0, i);
        std::cout << std::string(cSize.width, ' ');

        // Gutter: right-aligned line number + vertical bar
        GoTo(0, i);
        std::cout << std::setw(6) << std::right << (idx + 1)
            << " " << char(179);

        // Text content starts at column 10
        GoTo(10, i);

        if (mode == Mode::Visual) {
            auto selLines = selText.GetLines();
            if (selLines[idx].completed) {
				std::cout << "\033[7m" << lines[idx] << "\033[0m";
            }
            else {
				std::cout << lines[idx];

                for (SelectedPos pos : selLines[idx].positions) {
					char c = GetCharAt(pos.currentX, i);
					GoTo(pos.currentX, i);
					std::cout << "\033[7m" << c << "\033[0m";
                }
            }
        }
        else {
			std::cout << lines[idx];
        }
    }

    // Editor info
    // Cleaning 
    GoTo(0, cSize.height - 1);
	std::cout << std::string(cSize.width, ' ');

    // Adding
    GoTo(0, cSize.height - 1);
    std::string modeStr;
    if (mode == Mode::Normal) modeStr = "Normal Mode";
    else if (mode == Mode::Edit) modeStr = "Edit Mode";
    else modeStr = "Visual Mode";
    
    std::cout << " " << modeStr;

    GoTo(currentX, currentY);

    std::cout << "\033[?25h";  // Show cursor
}

// Move cursor down by one line; scroll view if needed
bool WindowsView::IncreaseCurrentLine() {
    // Case A: at end of buffer window but not beyond last line
    if (startPoint + cSize.height - 1 == lines.size() &&
        currentLine + 1 <= lines.size()) {
        ++currentLine;
		XReAdjustment();
        GoTo(currentX, ++currentY);
        return true;
    }
    // Case B: space to move cursor down without scrolling
    else if (currentY < cSize.height - 5) {
        if (currentLine == lines.size())
            return false;  // Already at last line
        ++currentLine;
		XReAdjustment();
        GoTo(currentX, ++currentY);
        return true;
    }
    // Case C: scroll down one line and re-render
    else if (startPoint + cSize.height <= lines.size()) {
        ++currentLine;
        ++startPoint;
		XReAdjustment();
        if (mode != Mode::Visual)
			Render();
        return true;
    }

    return false;
}

// Move cursor up by one line; scroll view if needed
bool WindowsView::DecreaseCurrentLine() {
    // Case A: at top of view but not first line
    if (startPoint == 0 && currentLine > 1) {
        --currentLine;
		XReAdjustment();
        GoTo(currentX, --currentY);
        return true;
    }
    // Case B: space to move cursor up without scrolling
    else if (currentY > 5) {
        --currentLine;
		XReAdjustment();
        GoTo(currentX, --currentY);
        return true;
    }
    // Case C: scroll up one line and re-render
    else if (startPoint > 0) {
        --currentLine;
        --startPoint;
		XReAdjustment();
        Render();
        return true;
    }

    return false;
}

void WindowsView::XReAdjustment() {
	if (currentX + 3 > lines[currentLine - 1].size() + 10) {
        if (lines[currentLine - 1].size() == 1) {
            currentX = 10;
            return;
        }
        currentX = lines[currentLine - 1].size() + 10 - 2;
        return;
	}

    if (lines[currentLine - 1].size() + 10 >= maxX + 3) 
        currentX = maxX;
    else
        currentX = lines[currentLine - 1].size() + 10 - 2;
}

bool WindowsView::GoRight() {
    if (currentX + 3 > lines[currentLine - 1].size() + 10) return false;
    
    GoTo(++currentX, currentY);
    if (currentX > maxX) 
        maxX = currentX;

    return true;
}

void WindowsView::GoAllRight() {
	while (true) {
		if (currentX + 3 > lines[currentLine - 1].size() + 10) return;
        GoRight();
	}
}

bool WindowsView::GoLeft() {
    if (currentX == 10) return false;
    GoTo(--currentX, currentY);
    maxX = currentX;
    return true;
}

void WindowsView::GoAllLeft() {
	while (true) {
        if (currentX == 10) return;
        GoLeft();
	}
}

void WindowsView::ChangeMode(const Mode _mode) {
    if (_mode == Mode::Normal || _mode == Mode::Visual)
        std::cout << "\033[1 q"; 
    else std::cout << "\033[6 q";
    mode = _mode;
}

void WindowsView::GoTop() {
	while (true) {
        DecreaseCurrentLine();
        if (currentLine == 1) break;
	}
}

void WindowsView::GoBottom() {
	while (true) {
        IncreaseCurrentLine();
        if (currentLine == lines.size()) break;
	}
}

int WindowsView::GetCurrentX() {
    return currentX;
}

void WindowsView::SetCurrentX(int x) {
    currentX = x;
    GoTo(currentX, currentY);
    maxX = currentX;
}

int WindowsView::GetCurrentLine() {
    return currentLine;
}

void WindowsView::SetCurrentLine(int line) {
    currentLine = line;
}

int WindowsView::GetCurrentStart() {
    return startPoint;
}

void WindowsView::SetCurrentStart(int start) {
    startPoint = start;
}

int WindowsView::GetCurrentY() {
    return currentY;
}

void WindowsView::SetCurrentY(int y) {
    currentY = y;
    GoTo(currentX, currentY);
}

void WindowsView::UpdateLines(std::vector<std::string>& _lines) {
    lines = _lines;
}

const std::vector<std::string>& WindowsView::GetLines() {
    return lines;
}

unsigned long int WindowsView::GetCurrentPos() const {
    unsigned long int currentPos = 0;
    for (int i = 0; i < currentLine - 1; i++)
        currentPos += lines[i].size();
    currentPos += currentX - 10;
    return currentPos;
}

void WindowsView::HighlightOne() {
	char c = GetCharAt(currentX, currentY);
	GoTo(GetCurrentX(), GetCurrentY());
	std::cout << "\033[7m" << c << "\033[0m";
}

void WindowsView::RemoveHighlight() {
	char c = GetCharAt(currentX, currentY);
	GoTo(GetCurrentX(), GetCurrentY());
	std::cout << c;
}

void WindowsView::VisualCommands(char c) {
	auto selLines = selText.GetLines();
    switch (c) {
    case 'j': { // Down
        if (mode == Mode::Visual && currentLine + 1 <= lines.size()) {
            // if the second next line is greater than the initial selected line
            // Logic: + 1 because selLines[i].currentLine brings currentLine - 1 and firstPosX brigs currentLine + 0
            if (selLines[currentLine].currentLine + 1 > selText.GetFirstPosX().currentLine) {
            	int tempX = currentX;
            	int tempMax = maxX;

                // if is the first line o the line before is not selected
                if (currentLine == 1 || selLines[currentLine - 2].startPos == -1) {
                    SelectedPos lineFirstPos = selLines[currentLine - 1].positions[0];
                    currentX = lineFirstPos.currentX;
                    GoTo(currentX, currentY);

                	selText.AddLine({ currentLine, currentX, currentX, false, { lineFirstPos } });

                    do selText.AddPos({ currentLine, currentX, currentY, GetCurrentPos() });
                    while (GoRight());
                }
                // if is not the first line or is not selected the line before
                else {
                    selText.AddLine({ currentLine, 10, currentX, true, {} });

                    GoAllLeft();
                    do selText.AddPos({ currentLine, currentX, currentY, GetCurrentPos() });
                    while (GoRight());
                }

            	currentX = tempX;
            	maxX = tempMax;
            	GoTo(currentX, currentY);
            }
            else {
                selText.RemoveLine({ currentLine, currentX, currentX, false, {} });
            }
        }
        if (!IncreaseCurrentLine()) break;

        if (mode == Mode::Visual) {
            // In the init selected line, if when we fall we land after the initial pos
            if (selLines[currentLine - 1].currentLine + 1 == selText.GetFirstPosX().currentLine) {
                if (currentX > selLines[currentLine - 1].positions[0].currentX) {
                    currentX = selLines[currentLine - 1].positions[0].currentX;
                    GoTo(currentX, currentY);
                }
            }

            if (selLines[currentLine - 1].currentLine + 1 > selText.GetFirstPosX().currentLine) {
            	selText.AddLine({ currentLine, currentX, currentX, false, {} });

                int tempX = currentX;
                int tempMax = maxX;

                do selText.AddPos({ currentLine, currentX, currentY, GetCurrentPos() });
                while (GoLeft());

                currentX = tempX;
                maxX = tempMax;

                GoTo(currentX, currentY);

                Render();
            } else {
                int tempX = currentX;
                int tempMax = maxX;

            	while (GoLeft())
					selText.RemovePos({ currentLine, currentX, currentY, 0 });

            	currentX = tempX;
                maxX = tempMax;

                GoTo(currentX, currentY);

                Render();
            }
        }
        break;
    }
    case 'k': { // Up 
        if (mode == Mode::Visual && currentLine - 1 > 0) {
            if (selLines[currentLine - 1].currentLine + 1 > selText.GetFirstPosX().currentLine) {
            	selText.RemoveLine({ currentLine, -1, -1, false, {} });
            }
        	else {
        		int tempX = currentX;
        		int tempMax = maxX;

                // if the is the last line or is the last selected line being the initial selected line
                if (currentLine == lines.size() || selLines[currentLine].startPos == -1) {
                    SelectedPos lineFirstPos = selLines[currentLine - 1].positions[0];
                    currentX = lineFirstPos.currentX;
                    GoTo(currentX, currentY);
                	selText.AddLine({ currentLine, currentX, currentX, false, { lineFirstPos } });

        			do selText.AddPos({ currentLine, currentX, currentY, GetCurrentPos() });
        			while (GoLeft());
                }
                else {
                	selText.AddLine({ currentLine, 10, currentX, true, {} });

                    GoAllRight();
        			do selText.AddPos({ currentLine, currentX, currentY, GetCurrentPos() });
        			while (GoLeft());
                }

        		currentX = tempX;
        		maxX = tempMax;
        		GoTo(currentX, currentY);
            }
        }
        if (!DecreaseCurrentLine()) break;
        if (mode == Mode::Visual) {
        	int tempX = currentX;
        	int tempMax = maxX;

            // In the init selected line, if when we fall we land before the initial pos
            if (selLines[currentLine - 1].currentLine + 1 == selText.GetFirstPosX().currentLine) {
                if (currentX < selLines[currentLine - 1].positions[0].currentX) {
                    currentX = selLines[currentLine - 1].positions[0].currentX;
                    tempX = currentX;
                    tempMax = currentX;
                	GoTo(currentX, currentY);
                }
            }

            if (selLines[currentLine - 1].currentLine + 1 >= selText.GetFirstPosX().currentLine) {
            	while (GoRight())
					selText.RemovePos({ currentLine, currentX, currentY, 0 });
            }
            else {
                selText.AddLine({ currentLine, currentX, currentX, false, {} });

                do selText.AddPos({ currentLine, currentX, currentY, GetCurrentPos() });
                while (GoRight());
            }
        	currentX = tempX;
        	maxX = tempMax;

        	GoTo(currentX, currentY);

        	Render();
        }
        break;
    }
    case 'l': { // right
        if (mode == Mode::Visual) {
            if (selLines[currentLine - 1].currentLine + 1 == selText.GetFirstPosX().currentLine) {
                if (currentX < selLines[currentLine - 1].startPos) {
                    selText.RemovePos({ currentLine, currentX, currentY, GetCurrentPos() });
                    RemoveHighlight();
                }
            }
            else if (selLines[currentLine - 1].currentLine + 1 > selText.GetFirstPosX().currentLine) {
                selText.AddPos({ currentLine, currentX, currentY, GetCurrentPos() });
                HighlightOne();
            }
            else {
                selText.RemovePos({ currentLine, currentX, currentY, GetCurrentPos() });
                RemoveHighlight();
            }
        }
        if (!GoRight()) break;
        if (mode == Mode::Visual) {
        	selText.AddPos({ currentLine, currentX, currentY, GetCurrentPos() });
        	HighlightOne();
        }
        break;
    }
    case 'h': { // left
        if (mode == Mode::Visual && currentX != 10) {
            if (selLines[currentLine - 1].currentLine + 1 == selText.GetFirstPosX().currentLine) {
                if (currentX > selLines[currentLine - 1].startPos) {
                    selText.RemovePos({ currentLine, currentX, currentY, GetCurrentPos() });
                    RemoveHighlight();
                }
            }
            else if (selLines[currentLine - 1].currentLine + 1 > selText.GetFirstPosX().currentLine) {
				selText.RemovePos({ currentLine, currentX, currentY, GetCurrentPos() });
				RemoveHighlight();
            } else {
        		selText.AddPos({ currentLine, currentX, currentY, GetCurrentPos() });
				HighlightOne();
            }
        }
        if (!GoLeft()) break;
        if (mode == Mode::Visual) {
        	selText.AddPos({ currentLine, currentX, currentY, GetCurrentPos() });
            HighlightOne();
        }
        break;
    }
    case 'i': {
        ChangeMode(Mode::Edit);
        Render();
        break;
    }
    case 'a': {
        currentX++;
        ChangeMode(Mode::Edit);
        Render();
        break;
    }
    case ':': {
        if (mode == Mode::Visual) break;
        char c = GetKey();
        if (c == 'q') {
            GoTo(0, 0);
            ClearScreen();
            file.close();
            exit(0);
        }

        if (c == 'w') {
            std::string fileText = pt.GetText();
            file << fileText;
        }
        break;
    }
    case 'g': {
        char c = GetKey();
        if (c == 'g') GoTop();
        break;
    }
    case 'G':
        GoBottom();
        break;

    case 'I': {
        GoAllLeft();
        ChangeMode(Mode::Edit);
        int lineN = GetCurrentLine() - 1;
        auto line = lines[lineN];
        int x = GetCurrentX() - 10;
        while (true) {
            if (line[x] != ' ' && line[x] != '\r') break;
            GoRight();
            x = GetCurrentX() - 10;
        }
        break;
    }
    case 'A': {
        GoAllRight();
        currentX++;
        ChangeMode(Mode::Edit);
        Render();
        break;
    }
    case 'w': {
        int lineN = GetCurrentLine() - 1;
        auto line = lines[lineN];
        int x = GetCurrentX() - 10;

        while (true) {
            x = GetCurrentX() - 10;
            if (!GoRight()) {
                IncreaseCurrentLine();
                GoAllLeft();
				lineN = GetCurrentLine() - 1;
				line = lines[lineN];
				x = GetCurrentX() - 10;

                if (line[x] != ' ' && line[x] != '\r') break;
                continue;
            }

            if (line[x + 1] == '.') break;
            if (line[x] == '.') break;
            if (line[x + 1] == '(') break;
            if (line[x + 1] == '<') break;
            if (line[x] == ' ' && line[x + 1] != ' ') break;
        }
        break;
    }
    case 'b': {
        int lineN = GetCurrentLine() - 1;
        auto line = lines[lineN];
        int x = GetCurrentX() - 10;

        while (true) {
            x = GetCurrentX() - 10;

            if (!GoLeft()) {
                if (GetCurrentLine() > 1) {
                    DecreaseCurrentLine();
                    GoAllRight();
                    lineN = GetCurrentLine() - 1;
                    line = lines[lineN];
                    continue;
                }
                break;
            }

            if (x - 1 < 0) {
                break;
            }

            if (line[x - 1] == '.') break;
            if (line[x] == '.') break;
            if (line[x - 1] == '(') break;
            if (line[x - 1] == '<') break;

            if (line[x] == ' ' && line[x - 1] != ' ') break;
        }
        break;
    }
    case 'v': {
        if (mode == Mode::Visual) break;
        ChangeMode(Mode::Visual);
        selText.AddLine({ currentLine, currentX, currentX, false, {} });
        selText.AddPos({ currentLine, currentX, currentY, GetCurrentPos() });
        selText.AddFirstPos(currentLine);
        selText.AddScrollPos(startPoint);
        Render();
        break;
    }
    
    case 'V': {
        if (mode == Mode::Visual) break;
        ChangeMode(Mode::Visual);
        break;
    }
    case 27: { // esc
        if (mode != Mode::Visual) break;
        ChangeMode(Mode::Normal);
        selText.Clear();
        Render();
        break;
    }
    default:
        break;
    }
}
