#include <vector>

#include "ConsoleUtils.h"
#include "PieceTable.h"
#include "fstream"
#include "WindowView.h"

void Render(std::vector<std::string>& lines, int start) {
	std::string content;

	ConsoleSize cSize = GetConsoleSize();
	for (int i = start; i < start + cSize.height - 1; i++) {
		content.append(lines[i]);
	}

	ClearScreen();
	std::cout << content;
	GoTo(0, 0);
}

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "Enter an argument";
		return 1;
	}

	std::ifstream file(argv[1]);

	if (!file.is_open()) {
		std::cerr << "No se pudo abrir el archivo";
		return 1;
	}

	std::string text;
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
		text.append(line + '\n');
		lines.emplace_back(line + '\n');
	}

	file.close();

	PieceTable pt = PieceTable(text);
	WindowsView view = WindowsView(lines);

	//int currentLine = 1;
	//int currentX = 0, currentY = 0;
	//Render(lines, currentLine - 1);

	view.Render();

	while (true) {
		switch (GetKey()) {
		case 'j': // Down
			view.IncreaseCurrentLine();
			break;
		case 'k': // Up
			view.DecreaseCurrentLine();
			break;
		case 'l': // Right
			view.GoRight();
			break;
		case 'h':
			view.GoLeft();
			break;
		case 'x':
			exit(0);

		default:
			break;
		}
	}
}


