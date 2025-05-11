#include "Command.h"
#include "ConsoleUtils.h"
#include "PieceTable.h"
#include "fstream"
#include "Testing.h"
#include "WindowView.h"
#include "Utils.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>\n";
		return 1;
	}

	std::ifstream file(argv[1], std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "No se pudo abrir el archivo\n";
		return 1;
	}

	const int TAB_SPACES = 4;
	std::string text;
	std::string line;
	while (std::getline(file, line)) {
		size_t pos = 0;
		while ((pos = line.find('\t', pos)) != std::string::npos) {
			line.replace(pos, 1, std::string(TAB_SPACES, ' '));
			pos += TAB_SPACES;
		}
		text.append(line);
		text.push_back('\n');
	}
	file.close();

	PieceTable pt(text);
	CommandManager cmdMan;
	auto lines = GetLines(pt.GetText());
	WindowsView view(lines);

	// Changing the cursor style to visual mode
	std::cout << "\033[1 q";
	while (true) {
		lines = GetLines(pt.GetText());
		view.UpdateLines(lines);

		view.Render();

		bool nose = false;

		while (true) {
			if (nose) break;

			if (view.mode == Mode::Visual) {
				char c = GetKey(false);
				switch (c) {
				case 'O': {
					nose = true;
					view.GoAllLeft();
					cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
						pt, view.GetCurrentPos(), std::string(1, '\n'), view
					));
					view.DecreaseCurrentLine();
					view.ChangeMode(Mode::Edit);
					break;
				}
				case 'o': {
					nose = true;
					view.GoAllRight();
					cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
						pt, view.GetCurrentPos(), std::string(1, '\n'), view
					));
					view.ChangeMode(Mode::Edit);
					view.GoAllLeft();
					break;
				}
				case 'u': {
					nose = true;
					cmdMan.undo();
					break;
				}
				case 'U': {
					nose = true;
					cmdMan.redo();
					break;
				}
				case 'x': {
					nose = true;
					cmdMan.executeCommand(std::make_unique<RemoveTextCommand>(
						pt, view.GetCurrentPos(), 1, view
					));
					break;
				}
				case 'd': {
					char c = GetKey(false);
					switch (c) {
					case 'd': {
						view.GoAllLeft();
						cmdMan.executeCommand(std::make_unique<RemoveLineCommand>(
							pt, view.GetCurrentPos(), view
						));

						if (lines.size() == view.GetCurrentLine()) {
							view.SetCurrentStart(view.GetCurrentStart() - 1);
							view.DecreaseCurrentLine();
							view.SetCurrentY(view.GetCurrentY() + 1);
						}
						nose = true;
						break;
					}
					case 'w': {
						int lineN = view.GetCurrentLine() - 1;
						auto line = lines[lineN];
						int x = view.GetCurrentX() - 10;

						int minCount = INT_MAX;

						int val = CountPosTillNext(line, x, '.');
						if (val > 0 && val < minCount)
							minCount = val;

						val = CountPosTillNext(line, x, ' ');
						if (val > 0 && val < minCount)
							minCount = val;

						val = CountPosTillNext(line, x, '(');
						if (val > 0 && val < minCount)
							minCount = val;

						val = CountPosTillNext(line, x, '<');
						if (val > 0 && val < minCount)
							minCount = val;

						if (line.size() < minCount + 1)
							minCount = line.size();

						cmdMan.executeCommand(std::make_unique<RemoveTextCommand>(
							pt, view.GetCurrentPos(), minCount, view
						));
						nose = true;
						break;
					}
					default:
						break;
					}
					break;
				}
				default:
					break;
				}

				view.VisualCommands(c);
			}
			else {
				char c = GetKey(false);
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
					int currPos = view.GetCurrentPos() - 1;
					if (currPos < 0) break;

					cmdMan.executeCommand(std::make_unique<RemoveTextCommand>(
						pt, currPos, 1, view
					));

					if (!view.GoLeft()) {
						if (lines.size() == view.GetCurrentLine()) {
							view.SetCurrentStart(view.GetCurrentStart() - 1);
							view.SetCurrentY(view.GetCurrentY() + 1);
						}

						view.DecreaseCurrentLine();
						view.GoAllRight();

						if (lines[view.GetCurrentLine() - 1].size() != 1)
							view.SetCurrentX(view.GetCurrentX() + 1);
					}

					break;
				}
				case '\t': { // tabs
					nose = true;
					cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
						pt, view.GetCurrentPos(), std::string(4, ' '), view
					));
					for (int i = 0; i < 3; i++)
						view.GoRight();
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
