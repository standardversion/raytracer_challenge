#include <cmath>
#include "plane.h"
#include "intersection.h"
#include "settings.h"


std::shared_ptr<Plane> Plane::create()
{
	return std::make_shared<Plane>();
}


void Plane::local_intersect(const ray_t& local_ray, intersections_t& intersections) const
{
	if (abs(local_ray.direction.y) > EPSILON)
	{
		const double t{ -local_ray.origin.y / local_ray.direction.y };
		intersections.add(t, this);
	}
}

tuple_t Plane::local_normal_at(const tuple_t& local_point) const
{
	return tuple_t::vector(0, 1, 0);
}