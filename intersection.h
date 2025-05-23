﻿#pragma once
#include <functional>
#include <optional>
#include <memory>
#include <vector>
#include <algorithm>
#include "intersection_state.h"

class Geometry;

/**
 * @struct intersection_t
 * @brief Represents the result of a ray intersecting a geometric object.
 *
 * Stores information about where along the ray the intersection occurs,
 * and which object was intersected. Used for hit testing and shading calculations.
 */
struct intersection_t
{
	/**
	 * @brief The time parameter (t-value) along the ray where the intersection occurs.
	 *
	 * A lower value indicates a closer intersection. Negative values may represent
	 * intersections behind the ray origin.
	 */
	double time{};

	/**
	 * @brief Barycentric alpha value (weight of the first vertex in a triangle).
	 */
	double alpha;

	/**
	 * @brief Barycentric beta value (weight of the second vertex in a triangle).
	 */
	double beta;

	/**
	 * @brief Barycentric gamma value (weight of the third vertex in a triangle).
	 */
	double gamma;

	/**
	 * @brief A pointer to the geometry object that was intersected.
	 *
	 * This is a shared pointer to a constant `Geometry` instance.
	 * It should never be null in a valid intersection.
	 */
	std::shared_ptr<const Geometry> object;

	/** @brief Default constructor. Initializes members to default values. */
	intersection_t() = default;

	/** @brief Copy constructor. */
	intersection_t(const intersection_t&) = default;

	/** @brief Copy assignment operator. */
	intersection_t& operator=(const intersection_t&) = default;

	/**
	 * @brief Constructs an intersection with a time value and object.
	 * @param time The distance along the ray where the intersection occurs.
	 * @param object The object that was intersected.
	 */
	intersection_t(double time, const std::shared_ptr<const Geometry>& object);

	/**
	 * @brief Constructs an intersection with time, object, and barycentric coordinates.
	 * @param time The distance along the ray where the intersection occurs.
	 * @param object The object that was intersected.
	 * @param alpha Barycentric coordinate for the first vertex.
	 * @param beta Barycentric coordinate for the second vertex.
	 * @param gamma Barycentric coordinate for the third vertex.
	 */
	intersection_t(double time, const std::shared_ptr<const Geometry>& object, double alpha, double beta, double gamma);

	/**
	 * @brief Compares this intersection object with another for equality.
	 *
	 * @param i The other intersection_t object to compare against.
	 * @return true if both intersection_t objects are considered equal, false otherwise.
	 *
	 * This operator checks whether the current object and the given intersection_t
	 * object have the same state (e.g., identical properties like position, normal, t-value, etc.).
	 */
	bool operator==(const intersection_t& i) const;

	/**
	 * @brief Prepares detailed shading information at the point of intersection.
	 *
	 * Constructs and returns an `intersection_state` object containing all necessary
	 * data for shading computations. This includes the point of intersection, the surface
	 * normal at that point, the eye (view) vector, the reflection vector, and a flag
	 * indicating whether the intersection occurs inside the object. It also calculates
	 * values related to refraction such as the over and under points and optionally
	 * the refractive indices before and after the intersection.
	 *
	 * @param r The ray that intersects the object.
	 * @param intersections A list of all intersections between the ray and scene geometry,
	 *                      used to determine context like refractive indices.
	 * @return An `intersection_state` structure populated with precomputed data for shading.
	 */
	intersection_state prepare(const ray_t& r, const intersections_t& intersections) const;
};

/**
 * @struct intersections_t
 * @brief Represents a collection of intersections between a ray and one or more objects.
 *
 * Used to store, manage, and process multiple intersection events (e.g., from a ray
 * intersecting several objects in a scene). Provides utilities to sort, query, and
 * determine the closest visible hit.
 */
struct intersections_t
{
	/**
	 * @brief A list of all intersection records.
	 *
	 * Each entry contains a `time` (t-value) and a pointer to the intersected object.
	 */
	std::vector<intersection_t> entries{};

	/**
	 * @brief Adds a single intersection to the collection.
	 *
	 * @param time The time or distance at which the intersection occurs.
	 * @param geo A pointer to the geometry involved in the intersection.
	 *
	 * Stores the intersection data (typically time and object pointer)
	 * for later processing, such as determining the closest visible hit.
	 */
	void add(const double time, const std::shared_ptr<const Geometry>& geo);

	/**
	 * @brief Adds a single triangle intersection with barycentric coordinates.
	 *
	 * @param time The time or distance at which the intersection occurs.
	 * @param geo A pointer to the geometry involved in the intersection.
	 * @param alpha Barycentric coordinate for the first triangle vertex.
	 * @param beta Barycentric coordinate for the second triangle vertex.
	 * @param gamma Barycentric coordinate for the third triangle vertex.
	 *
	 * This overload is used when working with smooth triangles and requires
	 * barycentric weights to enable normal interpolation at the intersection point.
	 */
	void add(const double time, const std::shared_ptr<const Geometry>& geo, const double alpha, const double beta, const double gamma);

	/**
	 * @brief Adds one or more intersections to the collection.
	 *
	 * @tparam Args Variadic template parameters allowing multiple intersection_t objects.
	 * @param i1 The first intersection to add.
	 * @param args Additional intersections to add recursively.
	 *
	 * This templated function enables batch addition of intersections.
	 * Useful for simplifying the addition of multiple intersection_t instances.
	 */
	template<typename... Args>
	void add(const intersection_t& i1, Args... args);

	/**
	 * @brief Sorts all stored intersections in ascending order by time (t-value).
	 *
	 * Typically used before determining the closest visible hit.
	 * After sorting, earlier intersections (i.e., closer to the ray origin) come first.
	 */
	void sort();

	/**
	 * @brief Finds the closest valid intersection (i.e., the "hit").
	 *
	 * @return The intersection_t representing the closest visible intersection
	 * (typically the one with the smallest positive `t` value). Returns a sentinel
	 * or null intersection if no valid hit exists.
	 */
	std::optional<intersection_t> hit(std::function<bool(const intersection_t& intersection)> filter) const;

	/**
	 * @brief Accesses an intersection at a specific index.
	 *
	 * @param i The index of the intersection to retrieve.
	 * @return The intersection_t at the specified index.
	 *
	 * Allows read-only indexed access to stored intersections, similar to array access.
	 * Throws or asserts if index is out of bounds (depending on implementation).
	 */
	intersection_t operator[](const std::size_t i) const;
};


/*
The add function is defined entirely in the header file.
The compiler needs to see the full definition of the template function to instantiate it when necessary.
Otherwise get linker error
*/
template<typename... Args>
void intersections_t::add(const intersection_t& i1, Args... args)
{
	entries.push_back(i1);
	if constexpr (sizeof...(args) > 0) {  // Check if there are more arguments
		add(args...);  // Recursively call add for the rest of the arguments
	}
	sort();
}