#include "LevelChunk.h"

LevelChunk::LevelChunk()
{
}

LevelChunk::LevelChunk(IntVector init[])
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

int LevelChunk::getChunkLength()
{
	int furthestPoint = -1;
	for (std::vector<IntVector> line : m_lines)
		for (IntVector point : line)
			if (point.x > furthestPoint)
				furthestPoint = point.x;
	assert(furthestPoint >= WIN_X);
	return furthestPoint;
}

int LevelChunk::getNumLines()
{
	return m_lines.size();
}
