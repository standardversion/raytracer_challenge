#pragma once
#include <vector>
#include <memory>
#include "colour.h"
#include "scene_object.h"
#include "geometry.h"
#include "light.h"
#include "ray.h"
#include "intersection.h"
#include "intersection_state.h"

class World
{
public:
	std::vector<std::unique_ptr<SceneObject>> scene_objects;
	std::vector<Geometry*> renderables;
	std::vector<Light*> lights;

	static World default_world();
	void add_object(std::unique_ptr<SceneObject> obj);
	void intersect(const ray_t& ray, intersections_t& intersections) const;
	colour_t shade_hit(const intersection_state& state) const;
	colour_t colour_at(const ray_t& ray) const;
};

