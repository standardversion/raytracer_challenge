#include <cmath>
#include "sphere.h"
#include "phong.h"
#include "intersection.h"


Sphere::Sphere(double r)
	: radius{r}
{ }

std::shared_ptr<Sphere> Sphere::create(double radius)
{

	return std::make_shared<Sphere>(radius);
}

std::shared_ptr<Sphere> Sphere::glass_sphere(double radius)
{
	auto s{ std::make_shared<Sphere>(radius) };
	auto phong = std::make_shared<Phong>();
	phong->transparency = 1.0;
	phong->refractive_index = 1.5;
	s->material = phong;
	return s;
}


void Sphere::local_intersect(const ray_t& local_ray, intersections_t& intersections) const
{
	const tuple_t origin{ tuple_t::point(0, 0, 0) };
	// sphere is at origin
	tuple_t Sphereo_ray{ local_ray.origin - origin };
	const double a{ tuple_t::dot(local_ray.direction, local_ray.direction) };
	const double b{ 2 * tuple_t::dot(local_ray.direction, Sphereo_ray) };
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
		intersections.add(t0, std::static_pointer_cast<const Geometry>(shared_from_this()));
		intersections.add(t1, std::static_pointer_cast<const Geometry>(shared_from_this()));
	}
}

tuple_t Sphere::local_normal_at(const tuple_t& local_point, const double alpha, const double beta, const double gamma) const
{
	return (local_point - tuple_t::point(0, 0, 0));
}