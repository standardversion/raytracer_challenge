#include <cmath>
#include <algorithm>
#include "cube.h"
#include "intersection.h"
#include "settings.h"


std::pair<double, double> Cube::check_axis(const double origin, const double direction) const
{
	const double tmin_numerator{ -1 - origin };
	const double tmax_numerator{ 1 - origin };
	double tmin;
	double tmax;
	if (abs(direction) >= EPSILON)
	{
		tmin = tmin_numerator / direction;
		tmax = tmax_numerator / direction;
	}
	else
	{
		tmin = tmin_numerator * INFINITY;
		tmax = tmax_numerator * INFINITY;
	}
	if (tmin > tmax)
	{
		std::swap(tmin, tmax);
	}
	return { tmin, tmax };
}

std::shared_ptr<Cube> Cube::create()
{
	return std::make_shared<Cube>();
}


void Cube::local_intersect(const ray_t& local_ray, intersections_t& intersections) const
{
	auto [xtmin, xtmax] {check_axis(local_ray.origin.x, local_ray.direction.x)};
	auto [ytmin, ytmax] {check_axis(local_ray.origin.y, local_ray.direction.y)};
	auto [ztmin, ztmax] {check_axis(local_ray.origin.z, local_ray.direction.z)};

	const double tmin{ std::max(std::max(xtmin, ytmin), ztmin) };
	const double tmax{ std::min(std::min(xtmax, ytmax), ztmax) };
	if (tmin > tmax)
	{
		return;
	}
	intersections.add(tmin, this);
	intersections.add(tmax, this);
}

tuple_t Cube::local_normal_at(const tuple_t& local_point, const double alpha, const double beta, const double gamma) const
{
	const double maxc{ std::max(std::max(abs(local_point.x), abs(local_point.y)), abs(local_point.z)) };
	if (maxc == abs(local_point.x))
	{
		return tuple_t::vector(local_point.x, 0, 0);
	}
	else if (maxc == abs(local_point.y))
	{
		return tuple_t::vector(0, local_point.y, 0);
	}
	else
	{
		return tuple_t::vector(0, 0, local_point.z);
	}
}