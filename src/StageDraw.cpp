#include "StageDraw.h"
#include "LevelChunk.h"
#include "draw.h"

void drawChunkLine(IntVector p1, IntVector p2, bool antiGrav)
{
	antiGrav ? drawLineColored(p1, p2, 255, 255, 255) : drawLine(p1, p2);
}

void drawChunk(const LevelChunk& chunk, int x)
{
	if (chunk.isAntiGrav())
	{
		drawRect({ x, 0, chunk.getChunkLength(), WIN_Y + 500 }, 0, 0, 0);
	}
	for (int i = 0; i < chunk.getNumLines(); i++)
	{
		for (int j = 0; j < chunk.getLineLength(i) - 1; j++)
		{
			IntVector p1 = chunk.getPoint(i, j);
			IntVector p2 = chunk.getPoint(i, j + 1);
			p1.x += x;
			p2.x += x;

			drawChunkLine(p1, p2, chunk.isAntiGrav());
			p1.y++;
			p2.y++;
			drawChunkLine(p1, p2, chunk.isAntiGrav());
			p1.x++;
			p2.x++;
			drawChunkLine(p1, p2, chunk.isAntiGrav());
			p1.y++;
			p2.y++;
			drawChunkLine(p1, p2, chunk.isAntiGrav());
			p1.x++;
			p2.x++;
			drawChunkLine(p1, p2, chunk.isAntiGrav());
			p1.y++;
			p2.y++;
			drawChunkLine(p1, p2, chunk.isAntiGrav());
			p1.x++;
			p2.x++;
			drawChunkLine(p1, p2, chunk.isAntiGrav());
			p1.y++;
			p2.y++;
			drawChunkLine(p1, p2, chunk.isAntiGrav());
			p1.x++;
			p2.x++;
			drawChunkLine(p1, p2, chunk.isAntiGrav());
		}
	}
}