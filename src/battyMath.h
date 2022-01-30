#ifndef MATH_H_INIT
#define MATH_H_INIT

#include "structs.h"

#define _USE_MATH_DEFINES
#include <math.h>

enum Orientation
{
	O_COLLINEAR,
	O_CLOCKWISE,
	O_COUNTERCLOCKWISE
};

double getDistancePointToLine(IntVector point, IntVector p1, IntVector p2);

#endif // !MATH_H_INIT
