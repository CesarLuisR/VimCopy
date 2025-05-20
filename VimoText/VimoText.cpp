#include "Command.h"
#include "fstream"
#include "ConsoleUtils.h"
#include "PieceTable.h"
#include "SelectedText.h"
#include "thread"
#include "Testing.h"
#include "WindowView.h"
#include "Utils.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>\n";
		return 1;
	}

	std::ifstream file(argv[1]);
	if (!file.is_open()) {
		std::cerr << "No se pudo abrir el archivo\n";
		return 1;
	}

	std::string filePath = std::string(argv[1]);
	std::ofstream outFile(filePath);
	if (!outFile.is_open()) {
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
	SelectedText selectedText(lines.size());
	WindowsView view(lines, selectedText, pt, outFile);

	// Changing the cursor style to normal mode
	std::cout << "\033[1 q";

	// Visual mode thread
	std::thread worker;

	while (true) {
		lines = GetLines(pt.GetText());
		view.UpdateLines(lines);
		selectedText.UpdateLines(lines.size());

		view.Render();

		bool nose = false;

		while (true) {
			if (nose) break;

			if (view.mode == Mode::Normal || view.mode == Mode::Visual) {
				char c = GetKey();
				switch (c) {
				case 'y': {
					if (view.mode == Mode::Visual) {
						int selectedFirstPos = selectedText.GetFirstPosX().curPos;
						int currentPos = view.GetCurrentPos();

						std::string text;
						if (selectedFirstPos < currentPos) {
							int length = view.GetCurrentPos() - selectedFirstPos + 1;
							text = pt.GetText(selectedFirstPos, length);
						} else {
							int length = (selectedFirstPos + 1) - view.GetCurrentPos();
							text = pt.GetText(currentPos, length);
						}
						CopyIntoClipboard(text);

						view.mode = Mode::Normal;
						nose = true;
					}

					break;
				}
				case 'p': {
					std::string clipboardText = PasteFromClipboard();

					cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
						pt, view.GetCurrentPos(), clipboardText, view
					));
					nose = true;
					break;
				}
				case 'O': {
					if (view.mode == Mode::Visual) break;
					nose = true;
					view.GoAllLeft();
					view.ChangeMode(Mode::Edit);

					int lineN = view.GetCurrentLine() - 1;
					auto line = lines[lineN];
					if (line.size() == 1) {
						cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
							pt, view.GetCurrentPos(), std::string(1, '\n'), view
						));
						view.DecreaseCurrentLine();
						break;
					}

					int currX = view.GetCurrentX();
					bool emptyLine = false;

					view.GoAllLeft();

					int x = view.GetCurrentX() - 10;
					while (true) {
						if (line[x] != ' ' && line[x] != '\r') break;
						if (!view.GoRight()) {
							emptyLine = true;
							break;
						}
						x = view.GetCurrentX() - 10;
					}

					view.SetCurrentX(currX);
					cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
						pt, view.GetCurrentPos(), std::string(1, '\n'), view
					));
					view.DecreaseCurrentLine();
					if (emptyLine || x == 0) break;

					cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
						pt, view.GetCurrentPos(), std::string(x + 1, ' '), view
					));

					lines = GetLines(pt.GetText());
					view.UpdateLines(lines);
					while (view.GoRight());

					break;
				}
				case 'o': {
					if (view.mode == Mode::Visual) break;
					nose = true;

					view.ChangeMode(Mode::Edit);
					view.GoAllRight();
					view.SetCurrentX(view.GetCurrentX() + 1);

					int lineN = view.GetCurrentLine() - 1;
					auto line = lines[lineN];
					if (line.size() == 1) {
						cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
							pt, view.GetCurrentPos(), std::string(1, '\n'), view
						));
						break;
					}

					int currX = view.GetCurrentX();
					bool emptyLine = false;

					view.GoAllLeft();

					int x = view.GetCurrentX() - 10;
					while (true) {
						if (line[x] != ' ' && line[x] != '\r') break;
						if (!view.GoRight()) {
							emptyLine = true;
							break;
						}
						x = view.GetCurrentX() - 10;
					}

					view.SetCurrentX(currX);
					cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
						pt, view.GetCurrentPos(), std::string(1, '\n'), view
					));

					if (emptyLine || x == 0) break;

					cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
						pt, view.GetCurrentPos(), std::string(x + 1, ' '), view
					));

					lines = GetLines(pt.GetText());
					view.UpdateLines(lines);
					while (view.GoRight());

					break;
				}
				case 'u': {
					if (view.mode == Mode::Visual) break;
					nose = true;
					cmdMan.undo();
					break;
				}
				case 'U': {
					if (view.mode == Mode::Visual) break;
					nose = true;
					cmdMan.redo();
					break;
				}
				case 'x': {
					if (view.mode == Mode::Visual) break;
					nose = true;
					cmdMan.executeCommand(std::make_unique<RemoveTextCommand>(
						pt, view.GetCurrentPos(), 1, view
					));
					break;
				}
				case 'd': {
					if (view.mode == Mode::Visual) {
						int selectedFirstPos = selectedText.GetFirstPosX().curPos;
						int currentPos = view.GetCurrentPos();

						int tempX = selectedText.GetFirstPosX().currentX;
						int tempY = selectedText.GetFirstPosX().currentY;
						int tempLine = selectedText.GetFirstPosX().currentLine;

						if (selectedFirstPos < currentPos) {
							int length = currentPos - selectedFirstPos;

							std::string text = pt.GetText(selectedFirstPos, length);
							CopyIntoClipboard(text);

							cmdMan.executeCommand(std::make_unique<RemoveTextCommand>(
								pt, selectedFirstPos, length + 1, view
							));

							view.SetCurrentLine(tempLine);
							view.SetCurrentX(tempX);
							view.SetCurrentY(tempY);
							view.SetCurrentStart(selectedText.GetScrollPos());
						} else {
							int length = selectedFirstPos - currentPos;

							std::string text = pt.GetText(currentPos, length);
							CopyIntoClipboard(text);

							cmdMan.executeCommand(std::make_unique<RemoveTextCommand>(
								pt, currentPos, length + 1, view
							));
						}

						view.mode = Mode::Normal;

						nose = true;
						break;
					}

					char c = GetKey();
					switch (c) {
					case 'd': {
						if (view.mode == Mode::Visual) break;
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
						if (view.mode == Mode::Visual) break;
						int lineN = view.GetCurrentLine() - 1;
						auto line = lines[lineN];
						int x = view.GetCurrentX() - 10;

						int minCount = INT_MAX;
						const char delims[] = {
							'.', ' ', '(', '<',
							'>', ')', ',', ';',
							':', '[', ']', '{',
							'}', '/', '\\', '=',
							'+', '-', '*', '%',
							'&', '|', '\'', '"'
						};

						for (char d : delims) {
							int val = CountPosTillNext(line, x, d);
							if (val > 0 && val < minCount) {
								minCount = val;
							}
						}

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
				char c = GetKey();
				switch (c) {
				case 27: // esc
					view.ChangeMode(Mode::Normal);
					view.Render();
					break;
				case 13: { // enter
					nose = true;

					int lineN = view.GetCurrentLine() - 1;
					auto line = lines[lineN];
					if (line.size() == 1) {
						cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
							pt, view.GetCurrentPos(), std::string(1, '\n'), view
						));
						break;
					}

					int currX = view.GetCurrentX();
					bool emptyLine = false;

					view.GoAllLeft();

					int x = view.GetCurrentX() - 10;
					while (true) {
						if (line[x] != ' ' && line[x] != '\r') break;
						if (!view.GoRight()) {
							emptyLine = true;
							break;
						}
						x = view.GetCurrentX() - 10;
					}

					view.SetCurrentX(currX);
					cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
						pt, view.GetCurrentPos(), std::string(1, '\n'), view
					));

					if (emptyLine || x == 0) break;

					cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
						pt, view.GetCurrentPos(), std::string(x + 1, ' '), view
					));

					lines = GetLines(pt.GetText());
					view.UpdateLines(lines);
					while (view.GoRight());

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
						pt, view.GetCurrentPos(), std::string(5, ' '), view
					));
					lines = GetLines(pt.GetText());
					view.UpdateLines(lines);

					while (view.GoRight());

					break;
				}
				default: // normal entry
					nose = true;
					cmdMan.executeCommand(std::make_unique<InsertTextCommand>(
						pt, view.GetCurrentPos(), std::string(1, c), view
					));
					view.GoRight();
					break;
				}
			}
		}
	}
}
