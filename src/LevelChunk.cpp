#include "LevelChunk.h"

LevelChunk::LevelChunk()
{
}

LevelChunk::LevelChunk(IntVector init[], bool antiGrav) : m_bAntiGrav(antiGrav)
{
	for (int i = 0; i < sizeof(init) / sizeof(IntVector); i++)
	{
		m_lines[i][0] = init[i];
	}
}

void LevelChunk::addLine(IntVector startingPoint)
{
	std::vector<IntVector> newLine = { startingPoint };
	m_lines.push_back(newLine);
}

void LevelChunk::addPointAtOffset(IntVector offset, int lineIdx)
{
	IntVector prevPoint = m_lines[lineIdx].back();
	m_lines[lineIdx].push_back({ prevPoint.x + offset.x, prevPoint.y + offset.y });
}

void LevelChunk::setAntiGrav(bool antigrav)
{
	m_bAntiGrav = antigrav;
}

int LevelChunk::getChunkLength() const
{
	int furthestPoint = -1;
	for (std::vector<IntVector> line : m_lines)
		for (IntVector point : line)
			if (point.x > furthestPoint)
				furthestPoint = point.x;
	assert(furthestPoint >= WIN_X);
	return furthestPoint;
}

int LevelChunk::getNumLines() const
{
	return m_lines.size();
}

int LevelChunk::getLineLength(int lineIdx) const
{
	return m_lines[lineIdx].size();
}

IntVector LevelChunk::getPoint(int lineIdx, int pointIdx) const
{
	return m_lines[lineIdx][pointIdx];
}

bool LevelChunk::isAntiGrav() const
{
	return m_bAntiGrav;
}
