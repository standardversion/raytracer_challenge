#include <cmath>
#include <algorithm>
#include "cylinder.h"
#include "intersection.h"
#include "settings.h"


void Cylinder::intersect_caps(const ray_t& local_ray, intersections_t& intersections) const
{
	if (!closed || abs(local_ray.direction.y) < EPSILON)
	{
		return;
	}
	double t{ (minimum - local_ray.origin.y) / local_ray.direction.y };
	if (check_cap(local_ray, t))
	{
		intersections.add(t, this);
	}
	t = (maximum - local_ray.origin.y) / local_ray.direction.y;
	if (check_cap(local_ray, t))
	{
		intersections.add(t, this);
	}
}

bool Cylinder::check_cap(const ray_t& local_ray, const double time) const
{
	const double x{ local_ray.origin.x + time * local_ray.direction.x };
	const double z{ local_ray.origin.z + time * local_ray.direction.z };
	return (pow(x, 2) + pow(z, 2)) <= 1;
}

std::shared_ptr<Cylinder> Cylinder::create()
{
	return std::make_shared<Cylinder>();
}


void Cylinder::local_intersect(const ray_t& local_ray, intersections_t& intersections) const
{
	const double a{ pow(local_ray.direction.x, 2) + pow(local_ray.direction.z, 2) };
	if (abs(a) < EPSILON)
	{
		intersect_caps(local_ray, intersections);
		return;
	}
	const double b{ 2 * local_ray.origin.x * local_ray.direction.x + 2 * local_ray.origin.z * local_ray.direction.z };
	const double c{ pow(local_ray.origin.x, 2) + pow(local_ray.origin.z, 2) -1 };
	const double discriminant{ pow(b, 2) - 4 * a * c };
	if (discriminant < 0)
	{
		return;
	}
	double t0{ (-b - std::sqrt(discriminant)) / (2 * a) };
	double t1{ (-b + std::sqrt(discriminant)) / (2 * a) };
	if (t0 > t1)
	{
		std::swap(t0, t1);
	}
	double y0{ local_ray.origin.y + t0 * local_ray.direction.y };
	if (minimum < y0 && y0 < maximum)
	{
		intersections.add(t0, this);
	}
	double y1{ local_ray.origin.y + t1 * local_ray.direction.y };
	if (minimum < y1 && y1 < maximum)
	{
		intersections.add(t1, this);
	}
	intersect_caps(local_ray, intersections);
	return;
}

tuple_t Cylinder::local_normal_at(const tuple_t& local_point) const
{
	const double distance{ pow(local_point.x, 2) + pow(local_point.z, 2) };
	if (distance < 1 && local_point.y >= maximum - EPSILON)
	{
		return tuple_t::vector(0, 1, 0);
	}
	else if (distance < 1 && local_point.y <= minimum + EPSILON)
	{
		return tuple_t::vector(0, -1, 0);
	}
	else
	{
		return tuple_t::vector(local_point.x, 0, local_point.z);
	}
}