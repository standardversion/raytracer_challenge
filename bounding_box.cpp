#include <vector>
#include "bounding_box.h"
#include "cube.h"
#include "intersection.h"

int bbox_tests = 0;

bbox_t& bbox_t::operator+=(const bbox_t& bb)
{
	add(bb.min);
	add(bb.max);
	return *this;
}

bool bbox_t::contains(const tuple_t& p) const
{
	return (min.x <= p.x && p.x <= max.x) &&
		(min.y <= p.y && p.y <= max.y) &&
		(min.z <= p.z && p.z <= max.z);
}

bool bbox_t::contains(const bbox_t& box) const
{
	return contains(box.min) && contains(box.max);
}

bbox_t bbox_t::transform(const matrix_t& m) const
{
	bbox_t new_box{};
	std::vector<tuple_t> points{
		min,
		tuple_t::point(min.x, min.y, max.z),
		tuple_t::point(min.x, max.y, min.z),
		tuple_t::point(min.x, max.y, max.z),
		tuple_t::point(max.x, min.y, min.z),
		tuple_t::point(max.x, min.y, max.z),
		tuple_t::point(max.x, max.y, min.z),
		max
	};
	for (const auto& p : points)
	{
		new_box.add(m * p);
	}
	return new_box;	
}

//bool bbox_t::intersect(const ray_t& r) const
//{
//	++bbox_tests;
//	auto cube = Cube::create();
//
//	// Construct transform from unit cube to this bounding box
//	// cube is 2 units in every dimension
//	const auto scale = matrix_t::scaling(
//		(max.x - min.x) / 2,
//		(max.y - min.y) / 2,
//		(max.z - min.z) / 2
//	);
//
//	const auto translate = matrix_t::translation(
//		(min.x + max.x) / 2,
//		(min.y + max.y) / 2,
//		(min.z + max.z) / 2
//	);
//
//	const matrix_t bbox_transform = translate * scale;
//	const matrix_t inverse_bbox_transform = bbox_transform.inverse();
//
//	// Transform the ray into the box's space
//	ray_t local_ray = r.transform(inverse_bbox_transform);
//
//	// Intersect with the unit cube at origin
//	intersections_t intersections;
//	cube->local_intersect(local_ray, intersections);
//
//	return !intersections.entries.empty();
//}

bool bbox_t::intersect(const ray_t& ray) const
{
	++bbox_tests;

	const double tmin_x = (min.x - ray.origin.x) * ray.inv_direction.x;
	const double tmax_x = (max.x - ray.origin.x) * ray.inv_direction.x;

	const double tmin_y = (min.y - ray.origin.y) * ray.inv_direction.y;
	const double tmax_y = (max.y - ray.origin.y) * ray.inv_direction.y;

	const double tmin_z = (min.z - ray.origin.z) * ray.inv_direction.z;
	const double tmax_z = (max.z - ray.origin.z) * ray.inv_direction.z;

	const double tmin = std::max(
		std::max(std::min(tmin_x, tmax_x), std::min(tmin_y, tmax_y)),
		std::min(tmin_z, tmax_z)
	);

	const double tmax = std::min(
		std::min(std::max(tmin_x, tmax_x), std::max(tmin_y, tmax_y)),
		std::max(tmin_z, tmax_z)
	);

	return tmin <= tmax;
}


tuple_t bbox_t::centroid() const
{
	return (min + max) * 0.5;
}

std::pair<bbox_t, bbox_t> bbox_t::split() const
{
	const double dx{ abs(max.x) + abs(min.x) };
	const double dy{ abs(max.y) + abs(min.y) };
	const double dz{ abs(max.z) + abs(min.z) };

	double x0{ min.x };
	double y0{ min.y };
	double z0{ min.z };

	double x1{ max.x };
	double y1{ max.y };
	double z1{ max.z };

	if (dx != dy || dx != dz || dy != dz)
	{
		if (dx > dy && dx > dz)
		{
			x0 = x1 = x0 + dx / 2.0;
		}
		else if (dy > dx && dy > dz)
		{
			y0 = y1 = y0 + dy / 2.0;
		}
		else
		{
			z0 = z1 = z0 + dz / 2.0;
		}
	}
	else
	{
		x0 = x1 = x0 + dx / 2.0;
	}
	tuple_t mid_min{ tuple_t::point(x0, y0, z0) };
	tuple_t mid_max{ tuple_t::point(x1, y1, z1) };
	const bbox_t left{ min, mid_max };
	const bbox_t right{ mid_min, max };
	return std::pair<bbox_t, bbox_t>{left, right};
}