#include "pattern.h"

colour_t pattern_t::at(const tuple_t& point) const
{
	return ((int)floor(point.x) % 2) ? b : a;
}

colour_t pattern_t::at_object(const Geometry* geo, const tuple_t& point) const
{
	const tuple_t obj_space_point{ geo->transform.inverse() * point };
	const tuple_t pattern_space_point{ transform.inverse() * obj_space_point };
	return at(pattern_space_point);
}