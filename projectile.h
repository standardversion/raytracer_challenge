#pragma once
#include "tuple.h"

struct projectile_t
{
	// MEMBER VARIABLES
	tuple_t position{ tuple_t::point(0, 0, 0) };
	tuple_t velocity{ tuple_t::vector(0, 0, 0) };
};
