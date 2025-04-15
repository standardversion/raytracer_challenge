#pragma once
#include "geometry.h"
#include "tuple.h"

struct intersection_state
{
	bool inside;
	double time;
	const Geometry* object;
	tuple_t point;
	tuple_t eye_vector;
	tuple_t normal;
};
