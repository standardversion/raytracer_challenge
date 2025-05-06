#include <cmath>
#include <algorithm>
#include "cylinder.h"
#include "intersection.h"
#include "settings.h"


void Cylinder::intersect_caps(const ray_t& local_ray, intersections_t& intersections) const
{
	// If the cylinder is open or the ray is parallel to the caps (y-direction), return early
	if (!closed || abs(local_ray.direction.y) < EPSILON)
	{
		return;
	}

	// Calculate the intersection time (t) for the bottom cap (minimum y-value)
	double t = (minimum - local_ray.origin.y) / local_ray.direction.y;

	// Check if the intersection lies within the radius of the cap and add it if valid
	if (check_cap(local_ray, t))
	{
		// Add the intersection with the bottom cap to the intersection list
		intersections.add(t, std::static_pointer_cast<const Geometry>(shared_from_this()));
	}

	// Calculate the intersection time (t) for the top cap (maximum y-value)
	t = (maximum - local_ray.origin.y) / local_ray.direction.y;

	// Check if the intersection lies within the radius of the cap and add it if valid
	if (check_cap(local_ray, t))
	{
		// Add the intersection with the top cap to the intersection list
		intersections.add(t, std::static_pointer_cast<const Geometry>(shared_from_this()));
	}
}

bool Cylinder::check_cap(const ray_t& local_ray, const double time) const
{
	// Calculate the x and z coordinates of the intersection point at the given time
	const double x = local_ray.origin.x + time * local_ray.direction.x;
	const double z = local_ray.origin.z + time * local_ray.direction.z;

	// Check if the intersection point lies within the cylinder's radius (1 unit)
	// The cylinder's radius is 1 (since it's normalized), so we check if (x^2 + z^2) <= 1
	return (pow(x, 2) + pow(z, 2)) <= 1;
}

std::shared_ptr<Cylinder> Cylinder::create()
{
	auto cylinder{ std::make_shared<Cylinder>() };
	cylinder->has_uvs = true;
	return cylinder;
}

uv_t Cylinder::get_uv(const tuple_t& point) const
{
	const double theta{ atan2(point.x , point.z) };
	const double raw_u{ theta / (2 * PI) };
	const double u{ 1 - (raw_u + 0.5) };
	const double v{ point.y - std::floor(point.y) };
	return { u, v };
}


void Cylinder::local_intersect(const ray_t& local_ray, intersections_t& intersections) const
{
	// Compute the coefficient 'a' based on the ray's direction.
	// This represents the projection of the ray's direction onto the XZ plane.
	const double a = pow(local_ray.direction.x, 2) + pow(local_ray.direction.z, 2);

	// If the ray is parallel to the axis of the cylinder (i.e., no XZ movement),
	// we don't check the surface but only check for intersections with the caps.
	if (abs(a) < EPSILON)
	{
		intersect_caps(local_ray, intersections); // Check for intersection with cylinder caps.
		return;
	}

	// Compute the coefficient 'b' for the quadratic equation.
	const double b = 2 * local_ray.origin.x * local_ray.direction.x + 2 * local_ray.origin.z * local_ray.direction.z;

	// Compute the coefficient 'c' for the quadratic equation.
	const double c = pow(local_ray.origin.x, 2) + pow(local_ray.origin.z, 2) - 1;

	// Calculate the discriminant to determine if the ray intersects the cylinder.
	const double discriminant = pow(b, 2) - 4 * a * c;

	// If the discriminant is negative, there's no intersection with the cylinder's surface.
	if (discriminant < 0)
	{
		return;
	}

	// Calculate the two possible intersection times (t0 and t1) using the quadratic formula.
	double t0 = (-b - std::sqrt(discriminant)) / (2 * a);
	double t1 = (-b + std::sqrt(discriminant)) / (2 * a);

	// Ensure t0 is the smaller value.
	if (t0 > t1)
	{
		std::swap(t0, t1);
	}

	// Compute the y-coordinate of the intersection points at times t0 and t1.
	double y0 = local_ray.origin.y + t0 * local_ray.direction.y;
	if (minimum < y0 && y0 < maximum) // Ensure the intersection is within the cylinder's height range.
	{
		// Add the intersection point at t0 to the list of intersections.
		intersections.add(t0, std::static_pointer_cast<const Geometry>(shared_from_this()));
	}

	double y1 = local_ray.origin.y + t1 * local_ray.direction.y;
	if (minimum < y1 && y1 < maximum) // Ensure the intersection is within the cylinder's height range.
	{
		// Add the intersection point at t1 to the list of intersections.
		intersections.add(t1, std::static_pointer_cast<const Geometry>(shared_from_this()));
	}
	intersect_caps(local_ray, intersections);
	return;
}

tuple_t Cylinder::local_normal_at(const tuple_t& local_point, const double alpha, const double beta, const double gamma) const
{
	const double distance{ pow(local_point.x, 2) + pow(local_point.z, 2) };

	// Check if the point is on the top cap (flat face at maximum y)
	if (distance < 1 && local_point.y >= maximum - EPSILON)
	{
		return tuple_t::vector(0, 1, 0);
	}
	// Check if the point is on the bottom cap (flat face at minimum y)
	else if (distance < 1 && local_point.y <= minimum + EPSILON)
	{
		return tuple_t::vector(0, -1, 0);
	}
	// Otherwise, the point is on the curved surface
	else
	{
		return tuple_t::vector(local_point.x, 0, local_point.z);
	}
}

bbox_t Cylinder::bounds() const
{
	tuple_t min{ minimum ? tuple_t::point(-1, minimum, -1) : tuple_t::point(-1, -INFINITY, -1) };
	tuple_t max{ maximum ? tuple_t::point(1, maximum, 1) : tuple_t::point(1, INFINITY, 1) };
	return bbox_t{ min, max };
}