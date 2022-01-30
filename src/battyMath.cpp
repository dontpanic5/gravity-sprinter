#include "battyMath.h"

int iMin(int x, int y)
{
	return x < y ? x : y;
}

int iMax(int x, int y)
{
	return x > y ? x : y;
}

/* Given three collinear points p, q, r, the function checks if
 * point q lies on line segment 'pr' */
static int onSegment(IntVector p, IntVector q, IntVector r)
{
	return q.x <= iMax(p.x, r.x) && q.x >= iMin(p.x, r.x) &&
		q.y <= iMax(p.y, r.y) && q.y >= iMin(p.y, r.y);
}

/* To find orientation of ordered triplet (p, q, r) */
int orientation(IntVector p, IntVector q, IntVector r)
{
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/
	// for details of below formula.
	int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

	if (val == 0) return O_COLLINEAR;

	return (val > 0) ? O_CLOCKWISE : O_COUNTERCLOCKWISE;
}

int areIntersecting(IntVector p1, IntVector q1, IntVector p2, IntVector q2)
{
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// general case
	if (o1 != o2 && o3 != o4)
		return 1;

	// Special Cases
	// p1, q1 and p2 are collinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1))
		return 1;
	// p1, q1 and q2 are collinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1))
		return 1;
	// p2, q2 and p1 are collinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2))
		return 1;
	// p2, q2 and q1 are collinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2))
		return 1;

	return 0;
}

double degToRad(double deg)
{
	return deg * M_PI / 180;
}

IntVector rotatePoint(IntVector p, IntVector center, double deg)
{
	IntVector rotatedPoint = {
		static_cast<int>(center.x + (p.x - center.x) * cos(degToRad(deg)) - (p.y - center.y) * sin(degToRad(deg))),
		static_cast<int>(center.y + (p.x - center.x) * sin(degToRad(deg)) + (p.y - center.y) * cos(degToRad(deg)))
	};
	return rotatedPoint;
}

double getDistancePointToLine(IntVector point, IntVector p1, IntVector p2)
{
	double slope = (double)(p2.y - p1.y) / (double)(p2.x - p1.x);
	double b = p1.y - slope * p1.x;
	return fabs(slope*point.x - point.y + b) / sqrt(slope * slope + 1);
}
