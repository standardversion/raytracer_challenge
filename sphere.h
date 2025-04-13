#pragma once
#include "ray.h"
#include "matrix.h"
#include "tuple.h"
#include "material.h"
#include "object.h"

//forward declaratio to avoid circular dep
struct intersection_t;
struct intersections_t;

static int sph_id_counter{ 1 };
class Sphere : public Object
{
	// MEMBER VARIABLES
public:
	double radius{ 1.0 };

	// CONSTRUCTOR
	/**
	 * @brief Constructs a sphere with a specified radius.
	 *
	 * @param r The radius of the sphere. Defaults to 1.0 if not specified.
	 *
	 * Initializes a sphere centered at the origin with radius `r`. The sphere may
	 * later be transformed or associated with material properties, depending on
	 * your implementation.
	 */
	Sphere(const double r = 1.0);

	// MEMBER FUNCTIONS
	/**
	 * @brief Computes intersections between the sphere and a given ray.
	 *
	 * @param ray The ray to test for intersection with this sphere.
	 * @param intersections A reference to an intersections_t collection where results will be stored.
	 *
	 * Determines the points (if any) at which the ray intersects the sphere, and
	 * adds those intersection points (typically as time values and object pointers)
	 * to the provided `intersections` collection. Handles rays originating inside
	 * or outside the sphere, and accounts for all valid real intersections.
	 */
	void intersect(const ray_t& ray, intersections_t& intersections) const override;


	tuple_t local_normal_at(const tuple_t& local_point) const override;

};
