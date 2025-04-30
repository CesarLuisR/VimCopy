#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <iostream>

enum class DataSource : std::uint8_t {
	Original, Append
};

struct Data {
	DataSource source;
	int index = 0;
	int length = 0;
};

struct Piece {
	int range = 0;
	Data data;
	std::unique_ptr<Piece> next = nullptr;
	Piece* prev = nullptr;
};

struct Sequence {
	std::unique_ptr<Piece> head;
	Piece* tail = nullptr;
	int range_counter = 0;
};

class PieceTable {
private:
	std::string original_buffer;
	std::string append_buffer;
	Sequence sequence;

public:
	PieceTable(const std::string& text);
	void InsertText(const std::string& text, int index);
	void RemoveText(int index, int length);

	static std::unique_ptr<Piece> createPiece(DataSource source, int index, int length);

	void Dump(const Piece* current) const;
	void DumpReverse(const Piece* current) const;
	const Piece* GetSeqHead() const;
	std::string GetText(int index, int length) const;
	std::string GetText() const;
};
