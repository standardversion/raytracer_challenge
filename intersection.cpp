#include <algorithm>
#include "intersection.h"
#include "settings.h"


bool intersection_t::operator==(const intersection_t& i) const
{
	return this->time == i.time && this->object == i.object;
}

intersection_state intersection_t::prepare(const ray_t& r) const
{
	intersection_state state{};
	state.time = time;
	state.object = object;
	state.point = r.position(time);
	state.eye_vector = -r.direction;
	state.normal = object->normal_at(state.point);
	if (tuple_t::dot(state.eye_vector, state.normal) < 0)
	{
		state.inside = true;
		state.normal = -state.normal;
	}
	else
	{
		state.inside = false;
	}
	state.reflect_vector = r.direction.reflect(state.normal);
	state.over_point = state.point + (state.normal * EPSILON);
	return state;
}

intersection_t intersections_t::operator[](const std::size_t i) const
{
	if (i < entries.size())
	{
		return entries[i];
	}
}


void intersections_t::add(const double time, const Geometry* geo)
{
	intersection_t intersection{ time, geo };
	entries.push_back(intersection);
	sort();
}

void intersections_t::sort()
{
	std::sort(entries.begin(), entries.end(), [](const intersection_t& a, const intersection_t& b) {
		return a.time < b.time;
	});
}

intersection_t intersections_t::hit()
{
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