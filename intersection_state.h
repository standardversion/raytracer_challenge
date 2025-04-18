#pragma once
#include "geometry.h"
#include "tuple.h"

/**
 * @struct intersection_state
 * @brief Stores precomputed information about a ray-object intersection for shading.
 *
 * This structure is typically generated from an `intersection_t` via the `prepare()` method.
 * It contains all the necessary data to perform lighting calculations, such as the point of
 * intersection, the surface normal, and the direction from which the ray approached.
 */
struct intersection_state
{
	/**
	 * @brief Indicates whether the intersection occurred from inside the object.
	 *
	 * If true, the ray entered the object, and the normal vector was inverted
	 * to point outward for correct shading.
	 */
	bool inside;

	/**
	 * @brief The time (t-value) at which the intersection occurred along the ray.
	 *
	 * Same as the time value from the originating `intersection_t`.
	 */
	double time;

	/**
	 * @brief A pointer to the intersected geometry object.
	 *
	 * Used to access material or geometric properties during shading.
	 */
	const Geometry* object;

	/**
	 * @brief The point in world space where the ray intersected the object.
	 */
	tuple_t point;

	/**
	 * @brief A point slightly above the intersection point, used to prevent shadow acne.
	 *
	 * This is calculated by offsetting the original `point` slightly along the surface normal.
	 * It ensures that shadow rays originate just above the surface, avoiding self-intersections
	 * due to floating-point precision errors.
	 */
	tuple_t over_point;

	/**
	 * @brief A vector pointing from the intersection point back toward the ray origin.
	 *
	 * Often used as the "eye" or "view" vector in lighting calculations.
	 */
	tuple_t eye_vector;

	tuple_t reflect_vector;

	/**
	 * @brief The surface normal at the point of intersection.
	 *
	 * Always points outward from the surface. If the ray entered the object,
	 * this vector is inverted to maintain consistency.
	 */
	tuple_t normal;
};

