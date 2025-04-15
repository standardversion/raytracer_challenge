#include <stdexcept>
#include "ray.h"

ray_t::ray_t(const tuple_t& o, const tuple_t& d)
{
	if (!o.is_point() || !d.is_vector())
	{
		std::invalid_argument("Origin has to be a point and Direction has to be a vector");
	}
	origin = o;
	direction = d;
}

tuple_t ray_t::position(const double time) const
{
	return origin + direction * time;
}

ray_t ray_t::transform(const matrix_t& m) const
{
	return {
		m * origin,
		m * direction
	};
}