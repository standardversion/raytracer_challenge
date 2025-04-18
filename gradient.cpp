#include "gradient.h"

Gradient::Gradient(const colour_t& a, const colour_t& b)
	: Pattern{ a, b }
{
}

colour_t Gradient::at(const tuple_t& point) const
{
	const colour_t distance{ b - a };
	const double fraction{ point.x - floor(point.x) };
	return a + distance * fraction;
}