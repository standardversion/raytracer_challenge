#pragma once
#include <memory>
#include "ray.h"
#include "intersection.h"
#include "bounding_box.h"
#include "triangle.h"

struct mesh_grp_t
{
	std::vector<std::shared_ptr<mesh_grp_t>> mesh_grps;

	std::vector<std::shared_ptr<Triangle>> triangles;

	bbox_t bbox;

	void build(int threshold);

	void add(std::shared_ptr<Triangle> t);

	void local_intersect(const ray_t& local_ray, intersections_t& intersections) const;
};