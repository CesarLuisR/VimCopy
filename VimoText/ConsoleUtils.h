#pragma once

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <termios.h>
#include <ncurses.h>
#endif

struct ConsoleSize {
	int width;
	int height;
};

ConsoleSize GetConsoleSize();
void GoTo(int x, int y);
char GetKey();
void ClearScreen();
char GetCharAt(int x, int y);