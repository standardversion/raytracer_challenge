#pragma once
#include <vector>

//forward declaratio to avoid circular dep
class Object;

struct intersection_t
{
	// MEMBER VARIABLES

	double time{};
	const Object* object{ nullptr };

	// OPERATORS

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
};

struct intersections_t
{
	// MEMBER VARIABLES

	std::vector<intersection_t> entries{};

	// MEMBER FUNCTIONS
	/**
	 * @brief Adds a single intersection to the collection.
	 *
	 * @param time The time or distance at which the intersection occurs.
	 * @param sph A pointer to the sphere involved in the intersection.
	 *
	 * Stores the intersection data (typically time and object pointer)
	 * for later processing, such as determining the closest visible hit.
	 */
	void add(const double time, const Object* sph);

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
	 * @brief Finds the closest valid intersection (i.e., the "hit").
	 *
	 * @return The intersection_t representing the closest visible intersection
	 * (typically the one with the smallest positive `t` value). Returns a sentinel
	 * or null intersection if no valid hit exists.
	 */
	intersection_t hit();

	// OPERATORS

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
}