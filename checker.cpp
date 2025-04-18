#include <cmath>
#include "checker.h"
#include "settings.h"

Checker::Checker(const colour_t& a, const colour_t& b)
	: Pattern{ a, b }
{
}

colour_t Checker::at(const tuple_t& point) const
{
	//If you're transforming the point from world → object → pattern space, any small matrix inaccuracies or
	//rounding errors might shift the sample point just enough to flip the checker's logic at the edges.
	//Use a small epsilon when comparing or explicitly round if appropriate
	return ((int)floor(point.x + EPSILON) + (int)floor(point.y + EPSILON) + (int)floor(point.z + EPSILON)) % 2 ? b : a;
}