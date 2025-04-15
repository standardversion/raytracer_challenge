#include <cmath>
#include "sphere.h"
#include "intersection.h"


Sphere::Sphere(const double r)
	: Geometry{}, radius{ r }
{
	
}

std::unique_ptr<Sphere> Sphere::create(double radius)
{
	//return std::make_unique<Sphere>(radius); // causes C2248
	return std::unique_ptr<Sphere>(new Sphere(radius));
}


void Sphere::intersect(const ray_t& ray, intersections_t& intersections) const
{
	ray_t transformed_ray{ ray.transform(transform.inverse()) };
	const tuple_t origin{ tuple_t::point(0, 0, 0) };
	// sphere is at origin
	tuple_t Sphereo_ray{ transformed_ray.origin - origin };
	const double a{ tuple_t::dot(transformed_ray.direction, transformed_ray.direction) };
	const double b{ 2 * tuple_t::dot(transformed_ray.direction, Sphereo_ray) };
	const double c{ tuple_t::dot(Sphereo_ray, Sphereo_ray) - std::pow(radius, 2) };

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

tuple_t Sphere::local_normal_at(const tuple_t& local_point) const
{
	return (local_point - tuple_t::point(0, 0, 0));
}