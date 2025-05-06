#include <memory>
#include <cmath>
#include "world.h"
#include "point_light.h"
#include "geometry.h"
#include "sphere.h"
#include "phong.h"
#include "matrix.h"
#include "group.h"

World World::default_world()
{
	World w{};
	auto sphere{ Sphere::create() };
	auto phong = std::dynamic_pointer_cast<Phong>(sphere->material);
	if (phong) {
		phong->colour = { 0.8, 1.0, 0.6 };
		phong->diffuse = 0.7;
		phong->specular = 0.2;
	}
	w.add_object(std::move(sphere));
	auto sphere2{ Sphere::create() };
	sphere2->transform = matrix_t::scaling(0.5, 0.5, 0.5);
	w.add_object(std::move(sphere2));
	auto light{ std::make_shared<PointLight>(colour_t{1, 1, 1}) };
	light->transform = matrix_t::translation(-10, 10, -10);
	w.add_object(std::move(light));
	return w;
}

void World::add_object(std::shared_ptr<SceneObject> obj)
{
	if (obj->renderable)
	{
		renderables.push_back(std::dynamic_pointer_cast<Geometry>(obj));
	}
	auto light = std::dynamic_pointer_cast<Light>(obj);
	if (light)
	{
		lights.push_back(light);
	}
	scene_objects.push_back(obj);
}

void World::intersect(const ray_t& ray, intersections_t& intersections) const
{
	for (auto& object : scene_objects)
	{
		auto group = dynamic_cast<Group*>(object.get());
		if (group)
		{
			group->intersect(ray, intersections);
		}
		else
		{
			auto geo = dynamic_cast<Geometry*>(object.get());
			if (geo)
			{
				geo->intersect(ray, intersections);
			}
		}
	}
}

colour_t World::shade_hit(const intersection_state& state, int remaining) const
{
	colour_t colour{ 0, 0, 0 };
	for (const auto& weak_light : lights)
	{
		if (auto light = weak_light.lock())
		{
			const double intensity{ light->intensity_at(state.over_point, *this) };
			colour += state.object->material->lighting(*light, state.object, state.point, state.eye_vector, state.normal, intensity);
			colour_t reflected_c{ reflected_colour(state, remaining) };
			colour_t refracted_c{ refracted_colour(state, remaining) };
			auto phong{ std::dynamic_pointer_cast<Phong>(state.object->material) };
			if (phong && phong->reflective > 0 && phong->transparency > 0)
			{
				const double reflectance{ state.schlick() };
				colour += reflected_c * reflectance + refracted_c * (1 - reflectance);
			}
			else
			{
				colour += reflected_c + refracted_c;
			}
		}
	}
	return colour;
}

colour_t World::reflected_colour(const intersection_state& state, int remaining) const
{
	// Start with black (no reflection by default)
	colour_t colour{ 0, 0, 0 };

	// Stop recursion if no bounces remain
	if (remaining <= 0)
	{
		return colour;
	}

	// Attempt to cast the object's material to a Phong material (supports reflection)
	auto phong = std::dynamic_pointer_cast<Phong>(state.object->material);

	// Only proceed if the material is reflective
	if (phong && phong->reflective > 0)
	{
		// Construct a reflection ray starting just above the surface to avoid self-intersection
		const ray_t reflected_ray{ state.over_point, state.reflect_vector };

		// Recursively compute the color returned by the reflected ray
		colour = colour_at(reflected_ray, remaining - 1);

		// Scale the reflected color by the material's reflectivity
		return colour * phong->reflective;
	}

	// Material is not reflective — return black
	return colour;
}


colour_t World::refracted_colour(const intersection_state& state, int remaining) const
{
	// Default color (white) — will be overwritten if refraction occurs
	colour_t colour{ 1, 1, 1 };

	// Stop recursion if we've reached the bounce limit
	if (remaining <= 0)
	{
		return colour_t{ 0, 0, 0 }; // No light contribution
	}

	// Try to get the Phong material (supports transparency)
	auto phong = std::dynamic_pointer_cast<Phong>(state.object->material);

	// Proceed only if the material is transparent
	if (phong && phong->transparency > 0)
	{
		// Compute ratio of refractive indices (from -> to)
		const double n_ratio = state.n1 / state.n2;

		// Cosine of the angle between eye vector and normal
		const double cos_i = tuple_t::dot(state.eye_vector, state.normal);

		// Compute sin²(t) using Snell's Law
		const double sin2_t = pow(n_ratio, 2) * (1 - pow(cos_i, 2));

		// Total internal reflection: no refraction possible
		if (sin2_t > 1)
		{
			return colour_t{ 0, 0, 0 }; // Fully reflected, no refraction contribution
		}

		// Compute cosine of the refracted angle
		const double cos_t = std::sqrt(1.0 - sin2_t);

		// Compute the direction of the refracted ray using vector refraction formula
		const tuple_t direction = state.normal * (n_ratio * cos_i - cos_t) - state.eye_vector * n_ratio;

		// Create the refracted ray starting just below the surface to avoid artifacts
		const ray_t refract_ray{ state.under_point, direction };

		// Recursively compute the refracted color, scaled by the material's transparency
		colour = colour_at(refract_ray, remaining - 1) * phong->transparency;
	}
	else
	{
		// No transparency, no refraction contribution
		return colour_t{ 0, 0, 0 };
	}

	// Return the calculated refracted color
	return colour;
}


colour_t World::colour_at(const ray_t& ray, int remaining) const
{
	colour_t colour{ 0, 0, 0 };
	intersections_t intersections{};
	intersect(ray, intersections);
	const auto intersection{ intersections.hit([](const intersection_t&) { return true; }) };
	if (intersection)
	{
		intersection_state state{ intersection->prepare(ray, intersections) };
		colour += shade_hit(state, remaining);
	}
	return colour;
}

bool World::is_shadowed(const tuple_t& point, const tuple_t& light_position) const
{
	tuple_t light_to_point{ light_position - point };
	const double distance{ light_to_point.magnitude() };
	light_to_point.normalize();
	const ray_t ray{ point, light_to_point };
	intersections_t intersections{};
	intersect(ray, intersections);
	const auto intersection{ intersections.hit([](const intersection_t& i) { 
			return i.object->cast_shadows;
		}
	) };
	return intersection && intersection->time < distance;
}