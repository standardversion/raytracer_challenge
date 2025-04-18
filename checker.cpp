#include <cmath>
#include "checker.h"

Checker::Checker(const colour_t& a, const colour_t& b)
	: Pattern{ a, b }
{
}

colour_t Checker::at(const tuple_t& point) const
{
	return ((int)floor(point.x) + (int)floor(point.y) + (int)floor(point.z)) % 2 ? b : a;
}