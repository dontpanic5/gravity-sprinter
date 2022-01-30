#ifndef LEVEL_CHUNK_H_INCLUDED
#define LEVEL_CHUNK_H_INCLUDED

#include "structs.h"
#include "main.h"
#include <vector>
#include <cassert>

class LevelChunk
{
public:
	LevelChunk();
	LevelChunk(IntVector init[], bool antiGrav = false);
	void addLine(IntVector startingPoint);
	void addPointAtOffset(IntVector offset, int lineIdx);
	void setAntiGrav(bool antigrav);
	int getChunkLength() const;
	int getNumLines() const;
	int getLineLength(int lineIdx) const;
	int getLineDistance(int lineIdx) const;
	IntVector getPoint(int lineIdx, int pointIdx) const;
	bool isAntiGrav() const;
protected:
	std::vector<std::vector<IntVector>> m_lines;
	bool m_bAntiGrav = false;
};

#endif