#include <cmath>
#include "ring.h"

Ring::Ring(const colour_t& a, const colour_t& b)
	: Pattern{ a, b }
{
}

colour_t Ring::at(const tuple_t& point) const
{
	return floor((int)std::sqrt(pow(point.x, 2) + pow(point.z, 2)) % 2) ? b : a;
}