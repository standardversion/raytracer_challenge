#pragma once
#include "colour.h"
#include "light.h"
#include "tuple.h"


class Material
{
protected:
	Material() = default;
public:
	virtual colour_t lighting(const Light& light, const tuple_t& position, const tuple_t& eye_vector, const tuple_t& normal_vector) const = 0;
	virtual bool operator==(const Material& m) const = 0;
};