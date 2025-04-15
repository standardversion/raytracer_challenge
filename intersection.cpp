#include "intersection.h"
#include <algorithm>


bool intersection_t::operator==(const intersection_t& i) const
{
	return this->time == i.time && this->object == i.object;
}

intersection_t intersections_t::operator[](const std::size_t i) const
{
	if (i < entries.size())
	{
		return entries[i];
	}
}


void intersections_t::add(const double time, std::shared_ptr<const Geometry> geo)
{
	intersection_t intersection{ time, geo };
	entries.push_back(intersection);
}

intersection_t intersections_t::hit()
{
	std::sort(entries.begin(), entries.end(), [](const intersection_t& a, const intersection_t& b) {
		return a.time < b.time;
	});
	intersection_t intersection;
	for (const auto& i : entries)
	{
		if (i.time < 0)
		{
			continue;
		}
		intersection = i;
		break;
	}
	return intersection;
}