#include "pattern.h"

Pattern::Pattern(const colour_t& a, const colour_t& b)
    : a{ a }, b{ b }
{
}

colour_t Pattern::at_object(const Geometry* geo, const tuple_t& point) const
{
	const tuple_t obj_space_point{ geo->world_to_object(point)};
	const tuple_t pattern_space_point{ transform.inverse() * obj_space_point };
	return at(pattern_space_point);
}