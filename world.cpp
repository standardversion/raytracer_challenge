#include <memory>
#include <cmath>
#include "world.h"
#include "light.h"
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
	Light light{ colour_t{1, 1, 1} };
	light.transform = matrix_t::translation(-10, 10, -10);
	w.add_object(std::make_unique<Light>(light));
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
			const bool in_shadow{ is_shadowed(state.over_point, light) };
			colour += state.object->material->lighting(*light, state.object, state.point, state.eye_vector, state.normal, in_shadow);
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
	colour_t colour{ 0, 0, 0 };
	if (remaining <= 0)
	{
		return colour;
	}
	auto phong{ std::dynamic_pointer_cast<Phong>(state.object->material) };
	if (phong && phong->reflective > 0)
	{
		const ray_t reflected_ray{ state.over_point, state.reflect_vector };
		colour = colour_at(reflected_ray, remaining - 1);
		return colour * phong->reflective;
	}
	return colour;
}

colour_t World::refracted_colour(const intersection_state& state, int remaining) const
{
	colour_t colour{ 1, 1, 1 };
	if (remaining <= 0)
	{
		return colour_t{ 0, 0, 0 };
	}
	auto phong{ std::dynamic_pointer_cast<Phong>(state.object->material) };
	if (phong && phong->transparency > 0)
	{
		const double n_ratio{ state.n1 / state.n2 };
		const double cos_i{ tuple_t::dot(state.eye_vector, state.normal) };
		const double sin2_t{ pow(n_ratio, 2) * (1 - pow(cos_i, 2)) };
		if (sin2_t > 1)
		{
			return colour_t{ 0, 0, 0 };
		}
		const double cos_t{ std::sqrt(1.0 - sin2_t) };
		const tuple_t direction{ state.normal * (n_ratio * cos_i - cos_t) - state.eye_vector * n_ratio };
		const ray_t refract_ray{ state.under_point, direction };
		colour = colour_at(refract_ray, remaining - 1) * phong->transparency;
	}
	else
	{
		return colour_t{ 0, 0, 0 };
	}
	return colour;
}

colour_t World::colour_at(const ray_t& ray, int remaining) const
{
	colour_t colour{ 0, 0, 0 };
	intersections_t intersections{};
	intersect(ray, intersections);
	const auto intersection{ intersections.hit() };
	if (intersection)
	{
		intersection_state state{ intersection->prepare(ray, intersections) };
		colour += shade_hit(state, remaining);
	}
	return colour;
}

bool World::is_shadowed(const tuple_t point, const std::weak_ptr<Light>& light) const
{
	if (auto light_shared_ptr = light.lock())
	{
		tuple_t light_to_point{ light_shared_ptr->position() - point };
		const double distance{ light_to_point.magnitude() };
		light_to_point.normalize();
		const ray_t ray{ point, light_to_point };
		intersections_t intersections{};
		intersect(ray, intersections);
		const auto intersection{ intersections.hit() };
		return intersection && intersection->time < distance;
	}
	else
	{
		return false;
	}
}