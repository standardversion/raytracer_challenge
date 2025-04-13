#pragma once
#include "colour.h"
#include "tuple.h"

struct light_t
{
	tuple_t position{};
	colour_t intensity{};
};