#include <memory>
#include "triangle.h"
#include "settings.h"
#include "intersection.h"

Triangle::Triangle(const tuple_t& p1, const tuple_t& p2, const tuple_t& p3)
	: p1{ p1 }, p2{ p2 }, p3{ p3 }, e1{ p2 - p1 }, e2{ p3 - p1 }
{
	normal = tuple_t::cross(e2, e1);
	normal.normalize();

}

std::shared_ptr<Triangle> Triangle::create(const tuple_t& p1, const tuple_t& p2, const tuple_t& p3)
{
	return std::make_shared<Triangle>(p1, p2, p3);
}


void Triangle::local_intersect(const ray_t& local_ray, intersections_t& intersections) const
{
	tuple_t dir_cross_e2{ tuple_t::cross(local_ray.direction, e2) };
	double determinant{ tuple_t::dot(e1, dir_cross_e2) };
	if (abs(determinant) < EPSILON)
	{
		return;
	}
	double f{ 1.0 / determinant };
	tuple_t p1_to_origin{ local_ray.origin - p1 };
	double u{ f * tuple_t::dot(p1_to_origin, dir_cross_e2) };
	if (u < 0 || u > 1)
	{
		return;
	}
	tuple_t origin_cross_e1{ tuple_t::cross(p1_to_origin, e1) };
	double v{ f * tuple_t::dot(local_ray.direction, origin_cross_e1) };
	if (v < 0 || (u + v) > 1)
	{
		return;
	}
	double t{ f * tuple_t::dot(e2, origin_cross_e1) };
	intersections.add(t, this);
	return;
}


tuple_t Triangle::local_normal_at(const tuple_t& local_point) const
{
	return normal;
}