#include "sphere.h"
#include "intersection.h"
#include <cmath>

// CONSTRUCTOR

sphere_t::sphere_t(const double r)
	: radius{ r }, id{ sph_id_counter++ }
{
	transform = matrix_t::identity();
}

// MEMBER FUNCTIONS

void sphere_t::intersect(const ray_t& ray, intersections_t& intersections) const
{
	ray_t transformed_ray{ ray.transform(transform.inverse()) };
	const tuple_t origin{ tuple_t::point(0, 0, 0) };
	// sphere is at origin
	tuple_t sphere_to_ray{ transformed_ray.origin - origin };
	const double a{ tuple_t::dot(transformed_ray.direction, transformed_ray.direction) };
	const double b{ 2 * tuple_t::dot(transformed_ray.direction, sphere_to_ray) };
	const double c{ tuple_t::dot(sphere_to_ray, sphere_to_ray) - std::pow(radius, 2) };

	const double discriminant{ std::pow(b, 2) - (4 * a * c) };
	if (discriminant >= 0)
	{
		double t0{ (-b - std::sqrt(discriminant)) / (2 * a) };
		double t1{ (-b + std::sqrt(discriminant)) / (2 * a) };

		if (t0 > t1)
		{
			std::swap(t0, t1);
		}
		intersections.add(t0, this);
		intersections.add(t1, this);
	}
}