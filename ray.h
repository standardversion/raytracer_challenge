#pragma once
#include "tuple.h"
#include "matrix.h"

/**
 * @brief Represents a ray in 3D space with an origin and direction.
 *
 * Rays are used to trace paths through the scene, test for intersections,
 * and calculate lighting and shading in ray tracing.
 */
struct ray_t
{
	/**
	 * @brief The origin point of the ray.
	 *
	 * This is where the ray starts in world space.
	 */
	tuple_t origin{};

	/**
	 * @brief The direction vector of the ray.
	 *
	 * Should be a normalized vector indicating the direction the ray travels.
	 */
	tuple_t direction{};

	/**
	 * @brief Inverse of the ray direction vector (1 / direction).
	 *
	 * Precomputed to speed up bounding box intersection tests by avoiding
	 * repeated division. Each component is the reciprocal of the corresponding
	 * ray direction component. If a direction component is zero, this should be
	 * handled appropriately to avoid division by zero.
	 */
	tuple_t inv_direction{};

	/**
	 * @brief Constructs a ray with a given origin and direction.
	 *
	 * @param o The origin of the ray, represented as a tuple (typically a point).
	 * @param d The direction of the ray, represented as a tuple (typically a vector).
	 *
	 * Initializes a ray starting from point `o` and extending infinitely in direction `d`.
	 * It is assumed that `d` is normalized or will be treated accordingly in computations.
	 */
	ray_t(const tuple_t& o, const tuple_t& d);

	/**
	 * @brief Computes a point along the ray at a given time.
	 *
	 * @param time A scalar value representing distance along the ray.
	 * @return The position on the ray at the specified time.
	 *
	 * Calculates the point `origin + direction * time`, which is used for tracing the
	 * path of the ray through space.
	 */
	tuple_t position(const double time) const;

	/**
	 * @brief Transforms the ray by a given transformation matrix.
	 *
	 * @param m The transformation matrix to apply to the ray.
	 * @return A new ray_t instance representing the transformed ray.
	 *
	 * Applies the matrix `m` to both the origin and direction of the ray.
	 * This is useful for moving rays between coordinate spaces (e.g., world to object space).
	 */
	ray_t transform(const matrix_t& m) const;
};