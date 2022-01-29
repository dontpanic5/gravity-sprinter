#ifndef LEVEL_CHUNK_H_INCLUDED
#define LEVEL_CHUNK_H_INCLUDED

#include "structs.h"
#include "main.h"
#include <vector>
#include <cassert>

enum class Bound { top, bottom };

class ChunkLine
{
public:
	ChunkLine(IntVector first, Bound bound);
	IntVector& operator[](int i);
protected:
	std::vector<IntVector> m_line;
	Bound m_bound;
};



//TODO ADD LINE CLASS
class LevelChunk
{
public:
	LevelChunk();
	LevelChunk(IntVector init[]);
	void addLine(IntVector startingPoint, Bound bound);
	void addPointAtOffset(IntVector offset, int lineIdx);
	int getChunkLength() const;
	int getNumLines() const;
	int getLineLength(int lineIdx) const;
	IntVector getPoint(int lineIdx, int pointIdx) const;
protected:
	std::vector<ChunkLine> m_lines;
};

#endif