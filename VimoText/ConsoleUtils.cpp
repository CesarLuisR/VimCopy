#include "ConsoleUtils.h"

ConsoleSize GetConsoleSize() {
	ConsoleSize size;
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		size.width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		size.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	}
	else {
		size.width = 0;
		size.height = 0;
	}
	return size;
#else
	struct winsize w;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
		size.width = w.ws_col;
		size.height = w.ws_row;
	}
	else {
		size.width = 0;
		size.height = 0;
	}
	return size;
#endif
}

void GoTo(int x, int y) {
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SetConsoleCursorPosition(hConsole, pos);
#else
	std::cout << "\033[" << y << ";" << x << "H";
#endif
}

char GetKey(bool nonBlocking = false) {
#ifdef _WIN32
	if (nonBlocking) {
		if (_kbhit()) return _getch();
		return 0; // Nada presionado
	}

	return _getch(); // Bloqueante
#else
	struct termios oldt, newt;
	char ch = 0;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	int oldFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
	if (nonBlocking) {
		fcntl(STDIN_FILENO, F_SETFL, oldFlags | O_NONBLOCK);
	}

	int result = read(STDIN_FILENO, &ch, 1);

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldFlags);

	return (result > 0) ? ch : 0;
#endif
}

void ClearScreen() {
#ifdef _WIN32
	std::system("cls");
#else
	std::system("clear");
#endif
}
