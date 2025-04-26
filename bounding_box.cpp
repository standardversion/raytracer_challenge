#include <vector>
#include "bounding_box.h"

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