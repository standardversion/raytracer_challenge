#pragma once
#include "matrix.h"
#include "material.h"
#include "ray.h"

//forward declaratio to avoid circular dep
struct intersection_t;
struct intersections_t;

static int obj_id_counter{ 1 };
class Object
{
public:
	int id{ 0 };
	matrix_t transform{ matrix_t::identity() };
	material_t material{};

	virtual tuple_t local_normal_at(const tuple_t& local_point) const = 0;
	virtual void intersect(const ray_t& ray, intersections_t& intersections) const = 0;

	tuple_t normal_at(const tuple_t& point) const;
protected:
	Object();
};
