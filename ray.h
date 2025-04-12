#pragma once
#include "tuple.h"
#include "matrix.h"

struct ray_t
{
	// MEMBER VARIABLES
	tuple_t origin{};
	tuple_t direction{};

	// CONSTRUCTOR
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

	// MEMBER FUNCTIONS
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