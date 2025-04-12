#pragma once
#include <vector>

//forward declaratio to avoid circular dep
struct sphere_t;

struct intersection_t
{
	double time{};
	const sphere_t* object{};
};

struct intersections_t
{
	std::vector<intersection_t> entries{};

	// MEMBER FUNCTIONS
	void add(const double time, const sphere_t* sph);

	// OPERATORS
	intersection_t operator[](const std::size_t i) const;
};