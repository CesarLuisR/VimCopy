#include "WindowView.h"
#include <iomanip>

// WindowsView: renders text lines in a console with line numbers and cursor navigation.
WindowsView::WindowsView(std::vector<std::string>& _lines)
    : currentLine(1),    // 1-based index of the cursor line
    currentX(10),      // X position of the cursor (column)
    currentY(0),       // Y position of the cursor (row)
    maxX(10),
    startPoint(0),     // Index of first visible line in 'lines'
    lines(_lines)      // Reference to the text buffer
{
}

// Render the current view: clear screen, draw line numbers and content
void WindowsView::Render() {
    cSize = GetConsoleSize();  // Update console dimensions

    // Draw each visible line (excluding bottom status row)
    for (int i = 0; i < cSize.height - 1; ++i) {
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

    // Position the cursor at (currentX, currentY)
    GoTo(currentX, currentY);
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

void WindowsView::GoRight() {
    if (currentX + 3 > lines[currentLine - 1].size() + 10) return;
    
    GoTo(0, 0);
    std::cout << lines[currentLine - 1].size();
    GoTo(++currentX, currentY);
    if (currentX > maxX) 
        maxX = currentX;
}

void WindowsView::GoLeft()
{
    if (currentX == 10) return;
    GoTo(--currentX, currentY);
    maxX = currentX;
}

