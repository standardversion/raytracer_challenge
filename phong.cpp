#include <cmath>
#include "phong.h"
#include "settings.h"

colour_t Phong::lighting
(
	const Light& light,
	const Geometry* geo,
	const tuple_t& position,
	const tuple_t& eye_vector,
	const tuple_t& normal_vector,
	const bool in_shadow
) const
{
	colour_t material_coluur{ colour };
	if (pattern)
	{
		material_coluur = pattern->at_object(geo, position);
	}
	// combine the surface color with the light's color/intensity
	colour_t effective_colour{ material_coluur * light.intensity };
	// find the direction to the light source
	tuple_t light_vector{ light.position() - position };
	light_vector.normalize();
	// compute the ambient contribution
	const colour_t ambient_colour{ effective_colour * ambient };
	colour_t diffuse_colour{};
	colour_t specular_colour{};
	// light_dot_normal represents the cosine of the angle between the light vector and the normal vector.
	// A negative number means the light is on the other side of the surface.
	const double light_dot_normal{ tuple_t::dot(light_vector, normal_vector) };
	colour_t black{ 0, 0, 0 };
	if (light_dot_normal < 0 || in_shadow)
	{
		diffuse_colour = black;
		specular_colour = black;
	}
	else
	{
		// compute the diffuse contribution
		diffuse_colour = effective_colour * diffuse * light_dot_normal;
		// reflect_dot_eye represents the cosine of the angle between the reflection vector and the eye vector.
		// A negative number means the light reflects away from the eye.
		const tuple_t reflection_vector{ -light_vector.reflect(normal_vector) };
		const double reflect_dot_eye{ tuple_t::dot(reflection_vector, eye_vector) };
		if (reflect_dot_eye <= 0)
		{
			specular_colour = black;
		}
		else
		{
			// compute the specular contribution
			const double factor{ std::pow(reflect_dot_eye, shininess) };
			specular_colour = light.intensity * specular * factor;
		}
	}
	// Add the three contributions together to get the final shading
	return ambient_colour + diffuse_colour + specular_colour;
}

bool Phong::operator==(const Material& m) const
{
	// Try to cast to Phong
	const Phong* other = dynamic_cast<const Phong*>(&m);
	if (!other) return false; // Not the same type

	const double a_comp{ std::fabs(this->ambient - other->ambient) };
	const double d_comp{ std::fabs(this->diffuse - other->diffuse) };
	const double spec_comp{ std::fabs(this->specular - other->specular) };
	const double shine_comp{ std::fabs(this->shininess - other->shininess) };
	if (this->colour == other->colour && a_comp < EPSILON && d_comp < EPSILON && spec_comp < EPSILON && shine_comp < EPSILON)
	{
		return true;
	}
	return false;
}