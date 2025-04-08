#pragma once
#include "tuple.h"

struct environment_t
{
	// MEMBER VARIABLES
	tuple_t gravity{ tuple_t::vector(0, 0, 0) };
	tuple_t wind{ tuple_t::vector(0, 0, 0) };
};
