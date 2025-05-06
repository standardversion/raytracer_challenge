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

	// Basic intersection properties
	state.time = time;                            // Time (t) of this intersection
	state.object = object.get();                  // Pointer to the object hit
	state.point = r.position(time);               // Compute the point of intersection on the object
	state.eye_vector = -r.direction;              // Vector pointing back toward the eye (camera)

	// Compute the surface normal at the point of intersection
	state.normal = object->normal_at(state.point, alpha, beta, gamma);

	// Determine if the intersection occurred from inside the object
	if (tuple_t::dot(state.eye_vector, state.normal) < 0)
	{
		state.inside = true;                      // Ray is inside the object
		state.normal = -state.normal;             // Invert normal to always face against the incoming ray
	}
	else
	{
		state.inside = false;                     // Ray is outside the object
	}

	// Compute the reflection vector based on the incoming direction and surface normal
	state.reflect_vector = r.direction.reflect(state.normal);

	// Offset the intersection point slightly along the normal to avoid precision issues:
	// - over_point is used for shadow calculations (e.g., prevent self-shadowing)
	// - under_point is used for refraction rays (e.g., simulate rays starting just inside the object)
	state.over_point = state.point + (state.normal * EPSILON);
	state.under_point = state.point - (state.normal * EPSILON);

	// Used to track which objects the ray is inside of as it moves through intersections
	std::vector<const Geometry*> containers{};

	// Loop over all intersections to determine refractive indices n1 (from) and n2 (to)
	for (const auto& intersection : intersections.entries)
	{
		// Before modifying containers, check if we've reached this intersection
		if (intersection == *this)
		{
			// n1: refractive index of the material the ray is coming from
			if (containers.empty())
			{
				state.n1 = 1.0; // Air
			}
			else
			{
				const auto phong = std::dynamic_pointer_cast<Phong>(containers.back()->material);
				state.n1 = phong ? phong->refractive_index : 1.0;
			}
		}

		// Update the containers stack: add or remove object based on presence
		auto found = std::find(containers.begin(), containers.end(), intersection.object.get());
		if (found == containers.end())
		{
			// Ray is entering a new object
			containers.push_back(intersection.object.get());
		}
		else
		{
			// Ray is leaving an object
			containers.erase(found);
		}

		// After modifying containers, check again if we've reached this intersection
		if (intersection == *this)
		{
			// n2: refractive index of the material the ray is entering into
			if (containers.empty())
			{
				state.n2 = 1.0; // Air
			}
			else
			{
				const auto phong = std::dynamic_pointer_cast<Phong>(containers.back()->material);
				state.n2 = phong ? phong->refractive_index : 1.0;
			}
			break; // We're done after computing n1 and n2 for this intersection
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