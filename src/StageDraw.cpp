#include "StageDraw.h"
#include "LevelChunk.h"
#include "draw.h"

void drawChunk(const LevelChunk& chunk, int x)
{
	for (int i = 0; i < chunk.getNumLines(); i++)
	{
		for (int j = 0; j < chunk.getLineLength(i) - 1; j++)
		{
			IntVector p1 = chunk.getPoint(i + x, j);
			IntVector p2 = chunk.getPoint(i + x, j + 1);
			drawLine(p1, p2);
			p1.y++;
			p2.y++;
			drawLine(p1, p2);
			p1.x++;
			p2.x++;
			drawLine(p1, p2);
			p1.y++;
			p2.y++;
			drawLine(p1, p2);
			p1.x++;
			p2.x++;
			drawLine(p1, p2);
			p1.y++;
			p2.y++;
			drawLine(p1, p2);
			p1.x++;
			p2.x++;
			drawLine(p1, p2);
			p1.y++;
			p2.y++;
			drawLine(p1, p2);
			p1.x++;
			p2.x++;
			drawLine(p1, p2);
		}
	}
}