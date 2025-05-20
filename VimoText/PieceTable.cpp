#include "PieceTable.h"

PieceTable::PieceTable(const std::string& text) :
	original_buffer(text)
{
	auto initialPiece = createPiece(DataSource::Original, 0, text.size());

	sequence.head = std::move(initialPiece);
	sequence.tail = sequence.head.get();
}

const Piece* PieceTable::GetSeqHead() const {
	return sequence.head.get();
}

void PieceTable::Dump(const Piece* current) const {
	while (current) {
		const std::string& buf =
			current->data.source == DataSource::Original
			? original_buffer
			: append_buffer;

		std::string slice = buf.substr(current->data.index, current->data.length);

		char prefix = (current->data.source == DataSource::Original) ? 'O' : 'A';
		std::cout << '[' << prefix << ':' << '"' << slice << '"' << "]";

		if (current->next)
			std::cout << " -> ";
		else
			std::cout << " -> NULL";

		current = current->next.get();
	}
	std::cout << std::endl;
}

void PieceTable::DumpReverse(const Piece* current) const {
	// If we pass the tail as current there is no need of doing this
	// But xd no time for it
	while (current && current->next) {
		current = current->next.get();
	}

	while (current) {
		const std::string& buf =
			current->data.source == DataSource::Original
			? original_buffer
			: append_buffer;

		std::string slice = buf.substr(current->data.index, current->data.length);

		char prefix = (current->data.source == DataSource::Original) ? 'O' : 'A';
		std::cout << '[' << prefix << ':' << '"' << slice << '"' << "]";

		if (current->prev)
			std::cout << " <- ";
		else
			std::cout << " <- NULL";

		current = current->prev;
	}
	std::cout << std::endl;
}

std::unique_ptr<Piece> PieceTable::createPiece(DataSource source, int index, int length) {
	auto piece = std::make_unique<Piece>();
	piece->range = 0;
	piece->data = { source, index, length };
	piece->next.reset(nullptr);
	return piece;
}

void PieceTable::InsertText(const std::string& text, int index) {
	append_buffer.append(text);

	auto current = sequence.head.get();
	size_t seqCurPos = 0;

	while (current) {
		// Detecting the piece for insertion
		if (seqCurPos + current->data.length > index) {
			int firstHalfLen = index - seqCurPos;
			auto lastHalf = createPiece(
				current->data.source,
				current->data.index + firstHalfLen,
				current->data.length - firstHalfLen
			);

			auto newPiece = createPiece(
				DataSource::Append,
				append_buffer.size() - text.size(),
				text.size()
			);

			// Splitting the piece
			current->data.length = firstHalfLen;

			// Last half links
			if (current->next) {
				lastHalf->next = std::move(current->next);
				lastHalf->next->prev = lastHalf.get();
			} else {
				sequence.tail = lastHalf.get();
			}

			lastHalf->prev = newPiece.get();

			// if lastHalf doesnt exist just not insert it
			if (lastHalf->data.length == 0) {
				lastHalf = std::move(current->next);
				if (lastHalf) lastHalf->prev = newPiece.get();
			}

			if (!lastHalf->next)
				sequence.tail = lastHalf.get();

			// if firstHalf doesnt exists
			newPiece->next = std::move(lastHalf);

			if (current->data.length == 0) {
				if (current->prev) {
					newPiece->prev = current->prev;  
					current->prev->next = std::move(newPiece);
				}
				else {
					newPiece->prev = nullptr;        
					sequence.head = std::move(newPiece);
				}
			} else {
				newPiece->prev = current;
				current->next = std::move(newPiece);
			}

			return;
		}

		// If index isnt in a piece and its in the end of the sequence
		if (current->next == nullptr) {
			auto newPiece = createPiece(
				DataSource::Append,
				append_buffer.size() - text.size(),
				text.size()
			);

			// inserting the new piece an the end
			newPiece->next = nullptr;
			newPiece->prev = current;
			current->next = std::move(newPiece);
			sequence.tail = newPiece.get();

			return;
		}

		seqCurPos += current->data.length;
		current = current->next.get();
	}
}

void PieceTable::RemoveText(int index, int length) {
	if (length == 0) return;

	Piece* current = sequence.head.get();
	int seqCurPos = 0;

	std::unique_ptr<Piece> firstPiece, lastPiece;

	Piece* prev = nullptr;
	while (current) {
		Piece* next = current->next.get();

		int pieceStart = seqCurPos;
		int pieceEnd = seqCurPos + current->data.length;

		// if the deletion span is between a piece
		if (index >= pieceStart && index + length <= pieceEnd) {
			auto firstSpan = createPiece(
				current->data.source,
				current->data.index,
				index - pieceStart
			);

			auto lastSpan = createPiece(
				current->data.source,
				current->data.index + ((index + length) - pieceStart),
				pieceEnd - (index + length)
			);

			// if both pieces exists
			if (firstSpan->data.length != 0 && lastSpan->data.length != 0) {
				lastSpan->prev = firstSpan.get();
				lastSpan->next = std::move(current->next);
				firstSpan->next = std::move(lastSpan);

				if (firstSpan->next->next)
					firstSpan->next->next->prev = firstSpan->next.get();
				else sequence.tail = firstSpan->next.get();

				if (current->prev) {
					firstSpan->prev = current->prev;
					current->prev->next = std::move(firstSpan);
				}
				else {
					firstSpan->prev = nullptr;
					sequence.head = std::move(firstSpan);
				}
				return;
			}

			// if both pieces does not exists
			if (firstSpan->data.length == 0 && lastSpan->data.length == 0) {
				if (prev) {
					prev->next = std::move(current->next);
					if (prev->next) {
						prev->next->prev = prev;
					}
				}
				else {
					sequence.head = std::move(current->next);
					if (sequence.head) {
						sequence.head->prev = nullptr;
					}
				}
				return;
			}

			// if only firstSpan exists
			if (firstSpan->data.length != 0) {
				firstSpan->next = std::move(current->next);
				if (firstSpan->next)
					firstSpan->next->prev = firstSpan.get();
				else sequence.tail = firstSpan.get();

				if (current->prev) {
					firstSpan->prev = current->prev;
					current->prev->next = std::move(firstSpan);
				}
				else {
					firstSpan->prev = nullptr;
					sequence.head = std::move(firstSpan);
				}
				return;
			}

			// if only lastSpan exists
			{
				lastSpan->next = std::move(current->next);
				if (lastSpan->next)
					lastSpan->next->prev = lastSpan.get();
				else sequence.tail = lastSpan.get();

				if (current->prev) {
					lastSpan->prev = current->prev;
					current->prev->next = std::move(lastSpan);
				}
				else {
					lastSpan->prev = nullptr;
					sequence.head = std::move(lastSpan);
				}
				return;
			}
		}

		// When removing is on various pieces
		// Beginning of the removing piece
		if (index >= pieceStart && index < pieceEnd) {
			firstPiece = createPiece(
				current->data.source,
				current->data.index,
				index - pieceStart
			);

			if (current->prev) {
				firstPiece->prev = current->prev;
				// current->prev-next just refers to current xd
				firstPiece->next = std::move(current->prev->next);
			} else {
				firstPiece->prev = nullptr;
				firstPiece->next = std::move(current->next);
			}
		}

		// End of the deletion piece
		if (index + length >= pieceStart && index + length <= pieceEnd) {
			lastPiece = createPiece(
				current->data.source,
				current->data.index + ((index + length) - pieceStart),
				pieceEnd - (index + length)
			);

			if (current->next) current->next->prev = lastPiece.get();
			else sequence.tail = current;

			lastPiece->next = std::move(current->next);
			break;
		}

		seqCurPos += current->data.length;
		prev = current;
		current = next;
	}

	// Joining the new pieces
	if (firstPiece == nullptr) return;

	if (lastPiece) {
		lastPiece->prev = firstPiece.get();

		if (lastPiece->next == nullptr)
			sequence.tail = lastPiece.get();

		firstPiece->next = std::move(lastPiece);

		if (firstPiece->prev) firstPiece->prev->next = std::move(firstPiece);
		else sequence.head = std::move(firstPiece);
	}
}

std::string PieceTable::GetText(int index, int length) const {
	std::string result;
	result.reserve(length);

	const Piece* current = sequence.head.get();
	int curPos = 0; 

	while (current && length > 0) {
		int pieceLen = current->data.length;
		int pieceStart = curPos;
		int pieceEnd = curPos + pieceLen;

		if (index < pieceEnd && index + length > pieceStart) {
			int startInPiece = std::max(0, index - pieceStart);
			int available = pieceLen - startInPiece;
			int toTake = std::min(available, length);

			const std::string& buf =
				current->data.source == DataSource::Original
				? original_buffer
				: append_buffer;

			result += buf.substr(
				current->data.index + startInPiece,
				toTake
			);

			length -= toTake;
			index += toTake;
		}

		curPos = pieceEnd;
		current = current->next.get();
	}

	return result;
}

std::string PieceTable::GetText() const {
	std::string result;

	const Piece* current = sequence.head.get();
	while (current) {
		const std::string& buf =
			current->data.source == DataSource::Original
			? original_buffer
			: append_buffer;

		result += buf.substr(
			current->data.index,
			current->data.length
		);

		current = current->next.get();
	}

	return result;
}
