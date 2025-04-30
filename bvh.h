#pragma once
#include <memory>
#include "ray.h"
#include "intersection.h"
#include "bounding_box.h"
#include "triangle.h"

struct bvh_t
{
	std::vector<std::shared_ptr<bvh_t>> bvhs;

	std::vector<std::shared_ptr<Triangle>> triangles;

	bbox_t bbox;

	void build(int threshold);

	void add(std::shared_ptr<Triangle> t);

	void local_intersect(const ray_t& local_ray, intersections_t& intersections) const;
};