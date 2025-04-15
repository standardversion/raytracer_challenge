#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include "intersection_state.h"

class Geometry;

/**
 * @brief Represents a single intersection between a ray and a geometric object.
 *
 * Used in ray tracing to record where and when a ray intersects with a shape in the scene.
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
	 * @brief A pointer to the geometry object that was intersected.
	 *
	 * This is a unique pointer to a constant `Geometry` instance.
	 * It should never be null in a valid intersection.
	 */
	const Geometry* object{ nullptr };

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

	intersection_state prepare(const ray_t& r) const;
};

struct intersections_t
{

	std::vector<intersection_t> entries{};

	/**
	 * @brief Adds a single intersection to the collection.
	 *
	 * @param time The time or distance at which the intersection occurs.
	 * @param sph A pointer to the sphere involved in the intersection.
	 *
	 * Stores the intersection data (typically time and object pointer)
	 * for later processing, such as determining the closest visible hit.
	 */
	void add(const double time, const Geometry* sph);

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

	void sort();

	/**
	 * @brief Finds the closest valid intersection (i.e., the "hit").
	 *
	 * @return The intersection_t representing the closest visible intersection
	 * (typically the one with the smallest positive `t` value). Returns a sentinel
	 * or null intersection if no valid hit exists.
	 */
	intersection_t hit();

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