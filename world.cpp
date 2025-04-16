#include <memory>
#include "world.h"
#include "light.h"
#include "geometry.h"
#include "sphere.h"
#include "phong.h"
#include "matrix.h"

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

void World::add_object(std::unique_ptr<SceneObject> obj)
{
	if (obj->renderable)
	{
		renderables.push_back(dynamic_cast<Geometry*>(obj.get()));
	}
	auto light = dynamic_cast<Light*>(obj.get());
	if (light)
	{
		lights.push_back(light);
	}
	scene_objects.push_back(std::move(obj));
}

void World::intersect(const ray_t& ray, intersections_t& intersections) const
{
	for (const auto object : renderables)
	{
		object->intersect(ray, intersections);
	}
}

colour_t World::shade_hit(const intersection_state& state) const
{
	colour_t colour{ 0, 0, 0 };
	for (const auto light : lights)
	{
		colour += state.object->material->lighting(*light, state.point, state.eye_vector, state.normal);
	}
	return colour;
}

colour_t World::colour_at(const ray_t& ray) const
{
	colour_t colour{ 0, 0, 0 };
	intersections_t intersections{};
	intersect(ray, intersections);
	const intersection_t intersection{ intersections.hit() };
	if (intersection.object)
	{
		const intersection_state state{ intersection.prepare(ray) };
		colour += shade_hit(state);
	}
	return colour;
}