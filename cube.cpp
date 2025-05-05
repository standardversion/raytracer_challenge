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
	auto cube{ std::make_shared<Cube>() };
	cube->has_uvs = true;
	return cube;
}

uv_t Cube::static_get_uv(const tuple_t& point)
{
	const Cube_Faces face{ Cube::face_from_point(point) };
	uv_t uv{};

	switch (face)
	{
	case Cube_Faces::left: // -X face
		uv.u = std::fmod(point.z + 1.0, 2.0) / 2.0;
		uv.v = std::fmod(point.y + 1.0, 2.0) / 2.0;
		break;

	case Cube_Faces::right: // +X face
		uv.u = std::fmod(1.0 - point.z, 2.0) / 2.0;
		uv.v = std::fmod(point.y + 1.0, 2.0) / 2.0;
		break;

	case Cube_Faces::front: // -Z face
		uv.u = std::fmod(point.x + 1.0, 2.0) / 2.0;
		uv.v = std::fmod(point.y + 1.0, 2.0) / 2.0;
		break;

	case Cube_Faces::back: // +Z face
		uv.u = std::fmod(1.0 - point.x, 2.0) / 2.0;
		uv.v = std::fmod(point.y + 1.0, 2.0) / 2.0;
		break;

	case Cube_Faces::up: // +Y face
		uv.u = std::fmod(point.x + 1.0, 2.0) / 2.0;
		uv.v = std::fmod(1.0 - point.z, 2.0) / 2.0;
		break;

	case Cube_Faces::down: // -Y face
		uv.u = std::fmod(point.x + 1.0, 2.0) / 2.0;
		uv.v = std::fmod(point.z + 1.0, 2.0) / 2.0;
		break;

	default:
		break;
	}
	return uv;
}

uv_t Cube::get_uv(const tuple_t& point) const
{
	return Cube::static_get_uv(point);
}

Cube_Faces Cube::face_from_point(const tuple_t& p)
{
	double coord{ std::max(std::max(abs(p.x), abs(p.y)), abs(p.z)) };
	if (coord == p.x) return Cube_Faces::right;
	if (coord == -p.x) return Cube_Faces::left;
	if (coord == p.y) return Cube_Faces::up;
	if (coord == -p.y) return Cube_Faces::down;
	if (coord == p.z) return Cube_Faces::front;

	return Cube_Faces::back;
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
	intersections.add(tmin, std::static_pointer_cast<const Geometry>(shared_from_this()));
	intersections.add(tmax, std::static_pointer_cast<const Geometry>(shared_from_this()));
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

bbox_t Cube::bounds() const
{
	return bbox_t{ tuple_t::point(-1, -1, -1), tuple_t::point(1, 1, 1) };
}