#include <cmath>
#include "plane.h"
#include "intersection.h"
#include "settings.h"


std::shared_ptr<Plane> Plane::create()
{
	auto plane{ std::make_shared<Plane>() };
	plane->has_uvs = true;
	return plane;
}

uv_t Plane::get_uv(const tuple_t& point) const
{

	const double u = point.x - std::floor(point.x); // Get fractional part of x
	const double v = point.z - std::floor(point.z); // Get fractional part of z

	return { u, v };
}


void Plane::local_intersect(const ray_t& local_ray, intersections_t& intersections) const
{
	// Check if the ray is not parallel to the plane
	// If the y component of the direction is nearly zero, the ray is parallel (no intersection)
	if (abs(local_ray.direction.y) > EPSILON)
	{
		// Calculate the intersection time (t) using the formula:
		// t = -origin.y / direction.y
		// This gives the point where the ray crosses y = 0 (the plane)
		const double t = -local_ray.origin.y / local_ray.direction.y;

		// Add the intersection with the plane at time t
		intersections.add(t, std::static_pointer_cast<const Geometry>(shared_from_this()));
	}
}

tuple_t Plane::local_normal_at(const tuple_t& local_point, const double alpha, const double beta, const double gamma) const
{
	return tuple_t::vector(0, 1, 0);
}

bbox_t Plane::bounds() const
{
	return bbox_t{ tuple_t::point(-INFINITY, 0, -INFINITY), tuple_t::point(INFINITY, 0, INFINITY) };
}