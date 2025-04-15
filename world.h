#pragma once
#include <vector>
#include <memory>
#include "scene_object.h"
#include "geometry.h"
#include "light.h"
#include "ray.h"
#include "intersection.h"

class World
{
public:
	std::vector<std::unique_ptr<SceneObject>> scene_objects;
	std::vector<Geometry*> renderables;
	std::vector<Light*> lights;

	static World default_world();
	void add_object(std::unique_ptr<SceneObject> obj);
	void intersect(const ray_t ray, intersections_t& intersections) const;
};

