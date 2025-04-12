#include "intersection.h"

intersection_t intersections_t::operator[](const std::size_t i) const
{
	if (i < entries.size())
	{
		return entries[i];
	}
}

void intersections_t::add(const double time, const sphere_t* sph)
{
	intersection_t intersection{ time, sph };
	entries.push_back(intersection);
}