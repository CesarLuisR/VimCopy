#include "Utils.h"

TabExpanderBuf::TabExpanderBuf(std::streambuf* src, int spaces)
    : source(src), spaceCount(spaces) {}

std::streambuf::int_type TabExpanderBuf::underflow() {
    if (pendingSpaces > 0) {
        bufferChar = ' ';
        --pendingSpaces;
        return traits_type::to_int_type(bufferChar);
    }

    int_type c = source->sbumpc();
    if (c == traits_type::eof()) return traits_type::eof();

    if (c == '\t') {
        pendingSpaces = spaceCount - 1;
        bufferChar = ' ';
        return traits_type::to_int_type(bufferChar);
    }

    bufferChar = traits_type::to_char_type(c);
    return traits_type::to_int_type(bufferChar);
}

std::vector<std::string> GetLines(const std::string& text) {
    std::vector<std::string> lines;
    std::string line;

    for (char c : text) {
        if (c == '\n') {
            line.push_back(c);
            lines.emplace_back(line);
            line.clear();
        }
        else {
            line.push_back(c);
        }
    }

    if (!line.empty()) {
        lines.emplace_back(line);
    }

    return lines;
}

int CountPosTillNext(const std::string& line, int x, char c) {
    size_t pos = line.find(c, x);
    if (pos == std::string::npos) return -1; 
    return static_cast<int>(pos - x);
}