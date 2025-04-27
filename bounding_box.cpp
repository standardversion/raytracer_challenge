#include <vector>
#include "bounding_box.h"
#include "cube.h"
#include "intersection.h"

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

bool bbox_t::intersect(const ray_t& r) const
{
	auto cube = Cube::create();

	// Construct transform from unit cube to this bounding box
	// cube is 2 units in every dimension
	const auto scale = matrix_t::scaling(
		(max.x - min.x) / 2,
		(max.y - min.y) / 2,
		(max.z - min.z) / 2
	);

	const auto translate = matrix_t::translation(
		(min.x + max.x) / 2,
		(min.y + max.y) / 2,
		(min.z + max.z) / 2
	);

	const matrix_t bbox_transform = translate * scale;
	const matrix_t inverse_bbox_transform = bbox_transform.inverse();

	// Transform the ray into the box's space
	ray_t local_ray = r.transform(inverse_bbox_transform);

	// Intersect with the unit cube at origin
	intersections_t intersections;
	cube->local_intersect(local_ray, intersections);

	return !intersections.entries.empty();
}