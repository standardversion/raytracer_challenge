#include <memory>
#include <stdexcept>
#include "triangle.h"
#include "settings.h"
#include "intersection.h"

int triangle_tests = 0;

Triangle::Triangle(const tuple_t& v1, const tuple_t& v2, const tuple_t& v3)
	: v1{ v1 }, v2{ v2 }, v3{ v3 }, e1{ v2 - v1 }, e2{ v3 - v1 }
{
	normal = tuple_t::cross(e2, e1);
	normal.normalize();
}

std::shared_ptr<Triangle> Triangle::create(const tuple_t& v1, const tuple_t& v2, const tuple_t& v3)
{
	return std::make_shared<Triangle>(v1, v2, v3);
}


void Triangle::local_intersect(const ray_t& local_ray, intersections_t& intersections) const
{
	++triangle_tests;
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
	if (n1 && n2 && n3)
	{
		return { n1.value() * alpha + n2.value() * beta + n3.value() * gamma };
	}
	return normal;
}

bbox_t Triangle::bounds() const
{
	bbox_t box{};
	box.add(v1, v2, v3);
	return box;
}

uv_t Triangle::get_uv(const tuple_t& point) const
{
	if (!has_uvs)
	{
		throw std::runtime_error("Triangle does not support UV mapping");
	}

	// Triangle vertices
	const tuple_t a = v1;
	const tuple_t b = v2;
	const tuple_t c = v3;

	// Edge vectors
	const tuple_t ab = b - a;
	const tuple_t ac = c - a;
	const tuple_t ap = point - a;

	// Full triangle area (ABC)
	const double area_abc = tuple_t::cross(ab, ac).magnitude();

	// Sub-triangle areas (using point P)
	const double area_bcp = tuple_t::cross(c - point, b - point).magnitude();
	const double area_cap = tuple_t::cross(a - point, c - point).magnitude();

	// Barycentric weights
	const double alpha = area_bcp / area_abc;
	const double beta = area_cap / area_abc;
	const double gamma = 1.0 - alpha - beta;

	// Interpolate UVs using barycentric coordinates
	const double u = alpha * v1_uv.first + beta * v2_uv.first + gamma * v3_uv.first;
	const double v = alpha * v1_uv.second + beta * v2_uv.second + gamma * v3_uv.second;

	return { u, v };
}
