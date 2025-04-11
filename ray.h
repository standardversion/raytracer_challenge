#pragma once
#include "tuple.h"

struct ray_t
{
	// MEMBER VARIABLES
	tuple_t origin{};
	tuple_t direction{};

	// CONSTRUCTOR
	ray_t(const tuple_t& o, const tuple_t& d);

	// MEMBER FUNCTIONS
	tuple_t position(const double time) const;
};