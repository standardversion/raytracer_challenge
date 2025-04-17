#include <cmath>
#include "plane.h"
#include "intersection.h"
#include "settings.h"


Plane::Plane()
	: Geometry{}
{

}

std::unique_ptr<Plane> Plane::create()
{
	//return std::make_unique<Plane>(); // causes C2248
	return std::unique_ptr<Plane>(new Plane());
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