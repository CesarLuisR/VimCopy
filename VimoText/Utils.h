#pragma once
#include <streambuf>
#include <string>
#include <vector>

class TabExpanderBuf : public std::streambuf {
public:
    TabExpanderBuf(std::streambuf* src, int spaces = 4);

protected:
    int_type underflow() override;

private:
    std::streambuf* source;
    int spaceCount;
    int pendingSpaces = 0;
    char bufferChar;
};

std::vector<std::string> GetLines(const std::string& text);
int CountPosTillNext(const std::string& line, int x, char c);