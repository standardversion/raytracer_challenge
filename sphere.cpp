#include <cmath>
#include "sphere.h"
#include "phong.h"
#include "intersection.h"
#include "settings.h"


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

uv_t Sphere::get_uv(const tuple_t& point) const
{
	// compute the azimuthal angle
	// -PI < theta <= PI
	// angle increases clockwise as viewed from above,
	// which is opposite of what we want, but we'll fix it later.
	const double theta{ atan2(point.x, point.z) };

	// vec is the vector pointing from the sphere's origin (the world origin)
	// to the point, which will also happen to be exactly equal to the sphere's
	// radius.
	const tuple_t vec{ tuple_t::vector(point.x, point.y, point.z) };
	const double radius{ vec.magnitude() };

	// compute the polar angle
	// 0 <= phi <= PI
	const double phi{ acos(point.y / radius) };

	// -0.5 < raw_u <= 0.5
	const double raw_u{ theta / (2 * PI) };

	// 0 <= u < 1
	// here's also where we fix the direction of u. Subtract it from 1,
	// so that it increases counterclockwise as viewed from above.
	const double u{ 1 - (raw_u + 0.5) };

	// we want v to be 0 at the south pole of the sphere,
	// and 1 at the north pole, so we have to "flip it over"
	// by subtracting it from 1.
	const double v{ 1 - phi / PI };

	return { u, v };
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

bbox_t Sphere::bounds() const
{
	return bbox_t{ tuple_t::point(-radius, -radius, -radius), tuple_t::point(radius, radius, radius) };
}