#if !defined(LEVEL_CHUNK_H_INCLUDED)
#define LEVEL_CHUNK_H_INCLUDED

#include "structs.h"
#include "main.h"
#include <vector>
#include <cassert>

class LevelChunk
{
public:
	LevelChunk();
	LevelChunk(IntVector init[]);
	void addLine(IntVector startingPoint);
	void addPointAtOffset(IntVector offset, int lineIdx);
	int getChunkLength();
	int getNumLines();
protected:
	std::vector<std::vector<IntVector>> m_lines;
};

#endif