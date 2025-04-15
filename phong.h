#pragma once
#include "material.h"

class Phong : public Material
{
public:
	colour_t colour{ 1, 1, 1 };
	double ambient{ 0.1 };
	double diffuse{ 0.9 };
	double specular{ 0.9 };
	double shininess{ 200.0 };
	Phong() = default;
	virtual colour_t lighting(const Light& light, const tuple_t& position, const tuple_t& eye_vector, const tuple_t& normal_vector) const;
	virtual bool operator==(const Material& m) const;
};