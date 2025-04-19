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

	tuple_t under_point;

	/**
	 * @brief A vector pointing from the intersection point back toward the ray origin.
	 *
	 * Often used as the "eye" or "view" vector in lighting calculations.
	 */
	tuple_t eye_vector;

	/**
	 * @brief The reflection vector at the point of intersection.
	 *
	 * Used in reflective shading models to trace reflected rays.
	 */
	tuple_t reflect_vector;

	/**
	 * @brief The surface normal at the point of intersection.
	 *
	 * Always points outward from the surface. If the ray entered the object,
	 * this vector is inverted to maintain consistency.
	 */
	tuple_t normal;

	/**
	 * @brief The refractive index of the medium the ray is exiting.
	 *
	 * Used in Snell's Law when calculating refraction.
	 */
	double n1;

	/**
	 * @brief The refractive index of the medium the ray is entering.
	 *
	 * Used in Snell's Law when calculating refraction.
	 */
	double n2;

	/**
	 * @brief Computes the reflectance at the intersection using the Schlick approximation.
	 *
	 * This method provides an efficient way to approximate the Fresnel effect,
	 * which determines how much light is reflected versus refracted at a surface.
	 *
	 * @return The reflectance coefficient (between 0 and 1).
	 */
	double schlick() const;
};

