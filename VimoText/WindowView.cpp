#include "WindowView.h"
#include <iomanip>

// WindowsView: renders text lines in a console with line numbers and cursor navigation.
WindowsView::WindowsView(std::vector<std::string>& _lines)
    : currentLine(1),    // 1-based index of the cursor line
    currentX(10),      // X position of the cursor (column)
    currentY(0),       // Y position of the cursor (row)
    maxX(10),
    startPoint(0),     // Index of first visible line in 'lines'
	mode(Mode::Visual),
    lines(_lines)      // Reference to the text buffer
{}

// Render the current view: clear screen, draw line numbers and content
void WindowsView::Render() {
    std::cout << "\033[?25l";  // Hide cursor
    cSize = GetConsoleSize();  // Update console dimensions

    // Draw each visible line (excluding bottom status row)
    for (int i = 0; i < cSize.height - 1; i++) {
        int idx = startPoint + i;
        if (idx >= lines.size())
            break;  // No more lines to display

        // Gutter: right-aligned line number + vertical bar
        GoTo(0, i);
        std::cout << std::string(cSize.width, ' ');
        GoTo(0, i);
        std::cout << std::setw(6) << std::right << (idx + 1)
            << " " << char(179);

        // Text content starts at column 10
        GoTo(10, i);
        std::cout << lines[idx];
    }

    GoTo(0, cSize.height - 1);
	std::cout << std::string(cSize.width, ' ');
    GoTo(0, cSize.height - 1);
    std::cout << (mode == Mode::Visual ? "Visual Mode" : "Edit Mode");

    GoTo(currentX, currentY);
    std::cout << "\033[?25h";  // Show cursor
}

// Move cursor down by one line; scroll view if needed
void WindowsView::IncreaseCurrentLine() {
    // Case A: at end of buffer window but not beyond last line
    if (startPoint + cSize.height - 1 == lines.size() &&
        currentLine + 1 <= lines.size()) {
        ++currentLine;
		XReAdjustment();
        GoTo(currentX, ++currentY);
    }
    // Case B: space to move cursor down without scrolling
    else if (currentY < cSize.height - 5) {
        if (currentLine == lines.size())
            return;  // Already at last line
        ++currentLine;
		XReAdjustment();
        GoTo(currentX, ++currentY);
    }
    // Case C: scroll down one line and re-render
    else if (startPoint + cSize.height <= lines.size()) {
        ++currentLine;
        ++startPoint;
		XReAdjustment();
        Render();
    }
}

// Move cursor up by one line; scroll view if needed
void WindowsView::DecreaseCurrentLine() {
    // Case A: at top of view but not first line
    if (startPoint == 0 && currentLine > 1) {
        --currentLine;
		XReAdjustment();
        GoTo(currentX, --currentY);
    }
    // Case B: space to move cursor up without scrolling
    else if (currentY > 5) {
        --currentLine;
		XReAdjustment();
        GoTo(currentX, --currentY);
    }
    // Case C: scroll up one line and re-render
    else if (startPoint > 0) {
        --currentLine;
        --startPoint;
		XReAdjustment();
        Render();
    }
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
    if (_mode == Mode::Visual)
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

void WindowsView::VisualCommands(char c) {
    switch (c) {
    case 'j': // Down
        IncreaseCurrentLine();
        break;
    case 'k': // Up
        DecreaseCurrentLine();
        break;
    case 'l':
        GoRight();
        break;
    case 'h':
        GoLeft();
        break;
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
        char c = GetKey();
        if (c == 'q') {
            GoTo(0, 0);
            ClearScreen();
            exit(0);
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
    default:
        break;
    }
}
