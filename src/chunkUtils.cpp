#include "chunkUtils.h"

int getChunkLength(IntVector* chunk[], int numLines)
{
	int furthestPoint = -1;
	for (int i = 0; i < numLines; i++)
	{
		for (int j = 0; j < sizeof(chunk[i]) / sizeof(IntVector); j++)
		{
			if (chunk[i][j].x > furthestPoint)
				furthestPoint = chunk[i][j].x;
		}
	}
	assert(furthestPoint >= WIN_X);
	return furthestPoint;
}