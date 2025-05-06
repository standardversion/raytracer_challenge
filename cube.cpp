#include <cmath>
#include <algorithm>
#include "cube.h"
#include "intersection.h"
#include "settings.h"


std::pair<double, double> Cube::check_axis(const double origin, const double direction) const
{
	// Compute the numerator for the minimum and maximum intersection times.
	// These are based on the cube's boundaries at -1 and +1 along the axis.
	const double tmin_numerator = -1 - origin;
	const double tmax_numerator = 1 - origin;

	double tmin;
	double tmax;

	// If the ray is not parallel to the axis (direction is non-zero), calculate the intersection times.
	if (abs(direction) >= EPSILON)
	{
		// Calculate the intersection times for the entry (tmin) and exit (tmax) along the axis.
		tmin = tmin_numerator / direction;
		tmax = tmax_numerator / direction;
	}
	else
	{
		// If the ray is parallel to the axis (direction is zero), set the times to infinity.
		// This implies the ray never intersects the cube along this axis, so no valid intersection.
		tmin = tmin_numerator * INFINITY;
		tmax = tmax_numerator * INFINITY;
	}

	// Ensure that tmin is the smaller value (entry time) and tmax is the larger value (exit time).
	// If tmin > tmax, the ray is exiting the cube from a different side than expected, so we swap them.
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
	// Determine which face of the cube the point lies on by calling Cube::face_from_point.
	const Cube_Faces face = Cube::face_from_point(point);
	uv_t uv{};

	switch (face)
	{
	case Cube_Faces::left: // -X face
		// The left face maps the z-coordinate and y-coordinate to u and v.
		// U is derived from the z-coordinate, and V is derived from the y-coordinate.
		uv.u = std::fmod(point.z + 1.0, 2.0) / 2.0;
		uv.v = std::fmod(point.y + 1.0, 2.0) / 2.0;
		break;

	case Cube_Faces::right: // +X face
		// The right face maps the z-coordinate and y-coordinate to u and v.
		// U is derived from the x-coordinate, and V is derived from the y-coordinate.
		uv.u = std::fmod(1.0 - point.z, 2.0) / 2.0;
		uv.v = std::fmod(point.y + 1.0, 2.0) / 2.0;
		break;

	case Cube_Faces::front: // -Z face
		// The front face maps the x-coordinate and y-coordinate to u and v.
		// U is derived from the x-coordinate, and V is derived from the y-coordinate.
		uv.u = std::fmod(point.x + 1.0, 2.0) / 2.0;
		uv.v = std::fmod(point.y + 1.0, 2.0) / 2.0;
		break;

	case Cube_Faces::back: // +Z face
		// The back face maps the x-coordinate and y-coordinate to u and v.
		// U is derived from the x-coordinate, and V is derived from the y-coordinate.
		uv.u = std::fmod(1.0 - point.x, 2.0) / 2.0;
		uv.v = std::fmod(point.y + 1.0, 2.0) / 2.0;
		break;

	case Cube_Faces::up: // +Y face
		// The up face maps the x-coordinate and z-coordinate to u and v.
		// U is derived from the x-coordinate, and V is derived from the z-coordinate.
		uv.u = std::fmod(point.x + 1.0, 2.0) / 2.0;
		uv.v = std::fmod(1.0 - point.z, 2.0) / 2.0;
		break;

	case Cube_Faces::down: // -Y face
		// The down face maps the x-coordinate and z-coordinate to u and v.
		// U is derived from the x-coordinate, and V is derived from the z-coordinate.
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
	// Check the intersections along each axis (X, Y, Z) by determining the min and max intersection times.
	// This will return the intersection times for each axis' boundaries.
	auto [xtmin, xtmax] = check_axis(local_ray.origin.x, local_ray.direction.x);
	auto [ytmin, ytmax] = check_axis(local_ray.origin.y, local_ray.direction.y);
	auto [ztmin, ztmax] = check_axis(local_ray.origin.z, local_ray.direction.z);

	// The ray intersects the cube when the intersection times along all three axes overlap.
	// We take the maximum of the minimum intersection times and the minimum of the maximum intersection times.
	const double tmin = std::max(std::max(xtmin, ytmin), ztmin);
	const double tmax = std::min(std::min(xtmax, ytmax), ztmax);

	// If the maximum intersection time is smaller than the minimum, there is no intersection.
	if (tmin > tmax)
	{
		return;
	}

	// Otherwise, the ray intersects the cube at two points: tmin (entry) and tmax (exit).
	// Add both intersection points to the list of intersections.
	intersections.add(tmin, std::static_pointer_cast<const Geometry>(shared_from_this()));
	intersections.add(tmax, std::static_pointer_cast<const Geometry>(shared_from_this()));
}

tuple_t Cube::local_normal_at(const tuple_t& local_point, const double alpha, const double beta, const double gamma) const
{
	// Find the axis with the maximum absolute value among x, y, or z to determine the face of the cube.
	const double maxc = std::max(std::max(abs(local_point.x), abs(local_point.y)), abs(local_point.z));

	// If the maximum absolute value is along the x-axis, the normal vector is along the x-axis.
	if (maxc == abs(local_point.x))
	{
		// Return the normal vector along the x-axis (sign is based on the cube's orientation).
		return tuple_t::vector(local_point.x, 0, 0);
	}
	// If the maximum absolute value is along the y-axis, the normal vector is along the y-axis.
	else if (maxc == abs(local_point.y))
	{
		// Return the normal vector along the y-axis (sign is based on the cube's orientation).
		return tuple_t::vector(0, local_point.y, 0);
	}
	// Otherwise, the maximum value must be along the z-axis.
	else
	{
		// Return the normal vector along the z-axis (sign is based on the cube's orientation).
		return tuple_t::vector(0, 0, local_point.z);
	}
}

bbox_t Cube::bounds() const
{
	return bbox_t{ tuple_t::point(-1, -1, -1), tuple_t::point(1, 1, 1) };
}