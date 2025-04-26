#include <memory>
#include "triangle.h"
#include "settings.h"
#include "intersection.h"

Triangle::Triangle(const tuple_t& v1, const tuple_t& v2, const tuple_t& v3)
	: v1{ v1 }, v2{ v2 }, v3{ v3 }, e1{ v2 - v1 }, e2{ v3 - v1 }
{
	normal = tuple_t::cross(e2, e1);
	normal.normalize();
}

Triangle::Triangle
(
	const tuple_t& v1,
	const tuple_t& v2,
	const tuple_t& v3,
	const std::pair<double, double>& v1_uv,
	const std::pair<double, double>& v2_uv,
	const std::pair<double, double>& v3_uv
)
	: v1{ v1 }, v2{ v2 }, v3{ v3 }, e1{ v2 - v1 }, e2{ v3 - v1 }, v1_uv{ v1_uv }, v2_uv{ v2_uv }, v3_uv{ v3_uv }
{
	normal = tuple_t::cross(e2, e1);
	normal.normalize();
}

std::shared_ptr<Triangle> Triangle::create(const tuple_t& v1, const tuple_t& v2, const tuple_t& v3)
{
	return std::make_shared<Triangle>(v1, v2, v3);
}

std::shared_ptr<Triangle> Triangle::create(const tuple_t& v1,
	const tuple_t& v2,
	const tuple_t& v3,
	const std::pair<double, double>& v1_uv,
	const std::pair<double, double>& v2_uv,
	const std::pair<double, double>& v3_uv
)
{
	return std::make_shared<Triangle>(v1, v2, v3, v1_uv, v2_uv, v3_uv);
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
	tuple_t p1_to_origin{ local_ray.origin - v1 };
	double beta{ f * tuple_t::dot(p1_to_origin, dir_cross_e2) };
	if (beta < 0 || beta > 1)
	{
		return;
	}
	tuple_t origin_cross_e1{ tuple_t::cross(p1_to_origin, e1) };
	double gamma{ f * tuple_t::dot(local_ray.direction, origin_cross_e1) };
	if (gamma < 0 || (beta + gamma) > 1)
	{
		return;
	}
	double t{ f * tuple_t::dot(e2, origin_cross_e1) };
	double alpha{ 1 - beta - gamma };
	intersections.add(t, std::static_pointer_cast<const Triangle>(shared_from_this()), alpha, beta, gamma);
	return;
}


tuple_t Triangle::local_normal_at(const tuple_t& local_point, const double alpha, const double beta, const double gamma) const
{
	return normal;
}

bbox_t Triangle::bounds() const
{
	bbox_t box{};
	box.add(v1, v2, v3);
	return box;
}