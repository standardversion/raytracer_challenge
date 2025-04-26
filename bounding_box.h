#pragma once
#include <cmath>
#include "tuple.h"

struct bbox_t
{
	tuple_t min{ tuple_t::point(INFINITY, INFINITY, INFINITY) };
	tuple_t max{ tuple_t::point(-INFINITY, -INFINITY, -INFINITY) };

	template<typename ...Args>
	void add(const tuple_t& p, Args ...args);

	bbox_t& operator+=(const bbox_t& bb);
};

template<typename... Args>
void bbox_t::add(const tuple_t& p, Args... args)
{
	if (p.x < min.x) min.x = p.x;
	if (p.y < min.y) min.y = p.y;
	if (p.z < min.z) min.z = p.z;
	if (p.x > max.x) max.x = p.x;
	if (p.y > max.y) max.y = p.y;
	if (p.z > max.z) max.z = p.z;
	if constexpr (sizeof...(args) > 0)
	{
		add(args...);
	}
}