#include <algorithm>
#include <stdexcept>
#include "intersection.h"
#include "settings.h"
#include "phong.h"

intersection_t::intersection_t(double time, const std::shared_ptr<const Geometry>& object)
	: time{ time }, object{ object }
{

}

intersection_t::intersection_t(double time, const std::shared_ptr<const Geometry>& object, double alpha, double beta, double gamma)
	: time{ time }, object{ object }, alpha{ alpha }, beta{ beta }, gamma{ gamma }
{ }

bool intersection_t::operator==(const intersection_t& i) const
{
	return this->time == i.time && this->object.get() == i.object.get();
}

intersection_state intersection_t::prepare(const ray_t& r, const intersections_t& intersections) const
{
	intersection_state state{};
	state.time = time;
	state.object = object.get();
	state.point = r.position(time);
	state.eye_vector = -r.direction;
	state.normal = object->normal_at(state.point, alpha, beta, gamma);
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
	state.under_point = state.point - (state.normal * EPSILON);

	std::vector<const Geometry*> containers{};
	for (const auto& intersection : intersections.entries)
	{
		if (intersection == *this)
		{
			if (containers.empty())
			{
				state.n1 = 1.0;
			}
			else
			{
				const auto phong{ std::dynamic_pointer_cast<Phong>(containers.back()->material)};
				if (phong)
				{
					state.n1 = phong->refractive_index;
				}
				else
				{
					state.n1 = 1.0;
				}
			}
		}
		auto found{ std::find(containers.begin(), containers.end(), intersection.object.get()) };
		if (found == containers.end())
		{
			containers.push_back(intersection.object.get());
		}
		else
		{
			containers.erase(found);
		}
		if (intersection == *this)
		{
			if (containers.empty())
			{
				state.n2 = 1.0;
			}
			else
			{
				const auto phong{ std::dynamic_pointer_cast<Phong>(containers.back()->material)};
				if (phong)
				{
					state.n2 = phong->refractive_index;
				}
				else
				{
					state.n2 = 1.0;
				}
			}
			break;
		}
	}

	return state;
}

intersection_t intersections_t::operator[](const std::size_t i) const
{
	if (i < entries.size())
	{
		return entries[i];
	}
	throw std::out_of_range("intersections_t index out of range");

}


void intersections_t::add(const double time, const std::shared_ptr<const Geometry>& geo)
{
	intersection_t intersection{ time, geo };
	entries.push_back(intersection);
	sort();
}

void intersections_t::add(const double time, const std::shared_ptr<const Geometry>& geo, const double alpha, const double beta, const double gamma)
{
	intersection_t intersection{ time, geo, alpha, beta, gamma };
	entries.push_back(intersection);
	sort();
}

void intersections_t::sort()
{
	std::sort(entries.begin(), entries.end(), [](const intersection_t& a, const intersection_t& b) {
		return a.time < b.time;
	});
}

std::optional<intersection_t> intersections_t::hit(std::function<bool(const intersection_t& intersection)> filter) const
{
	for (const auto& i : entries) {
		if (i.time >= 0 && filter(i)) {
			return i; // shared_ptr copy is safe
		}
	}
	return std::nullopt;
}