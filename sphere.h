#pragma once
#include "ray.h"

//forward declaratio to avoid circular dep
struct intersection_t;
struct intersections_t;

static int sph_id_counter{ 1 };
struct sphere_t
{
	// MEMBER VARIABLES
	double radius{ 1.0 };
	int id{ 0 };

	// CONSTRUCTOR
	sphere_t(const double r = 1.0);

	// MEMBER FUNCTIONS
	void intersect(const ray_t& ray, intersections_t& intersections) const;
};
