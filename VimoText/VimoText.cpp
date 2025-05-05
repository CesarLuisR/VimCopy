#include <vector>

#include "Command.h"
#include "ConsoleUtils.h"
#include "PieceTable.h"
#include "fstream"
#include "WindowView.h"

std::vector<std::string> getLines(const std::string& text) {
	std::vector<std::string> lines;
	std::string line;

	for (int i = 0; i < text.size(); i++) {
		if (text[i] == '\n') {
			line.push_back(text[i]);
			lines.emplace_back(line);
			line.clear();
		} 
		else {
			 line.push_back(text[i]);
		}
	}

	if (!line.empty()) {
		lines.emplace_back(line);
	}
	return lines;
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
	std::string line;
	while (std::getline(file, line))
		text.append(line + '\n');

	file.close();

	PieceTable pt(text);
	CommandManager cmdMan;

	auto lines = getLines(pt.GetText());
	WindowsView view = WindowsView(lines);

	// Changing the cursor style to visual mode
	std::cout << "\033[1 q"; 
	while (true) {
		lines = getLines(pt.GetText());
		view.UpdateLines(lines);

		view.Render();

		bool nose = false;

		while (true) {
			if (nose) break;

			if (view.mode == Mode::Visual)
				view.VisualCommands();
			else {
				char c = GetKey();
				switch (c) {
				case 27: // esc
					view.ChangeMode(Mode::Visual);
					view.Render();
					break;
				case 13: { // enter
					nose = true;
					cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
						pt, view.GetCurrentPos(), std::string(1, '\n'), view
					));
					break;
				}
				case 26: {  
					nose = true;
					cmdMan.undo();
					break;
				}
				case 25: {
					nose = true;
					cmdMan.redo();
					break;
				}
				case 8: { // backspace
					nose = true;
					cmdMan.executeCommand(std::make_unique<RemoveTextCommand>(
						pt, view.GetCurrentPos(), 1, view
					));
					view.GoLeft();
					break;
				}
				default: // normal entry
					nose = true;
					cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
						pt, view.GetCurrentPos(), std::string(1, c), view
					));
					break;
				}
			}
		}
	}
}


