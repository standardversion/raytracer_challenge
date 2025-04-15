#include <cmath>
#include "material.h"
#include "settings.h"

material_t::material_t(const colour_t c, const double a, const double d, const double spec, const double shine)
	: colour{ c }, ambient{ a }, diffuse{ d }, specular{ spec }, shininess{ shine }
{
	if (ambient < 0.0)
	{
		ambient = 0.0;
	}
	if (diffuse < 0.0)
	{
		diffuse = 0.0;
	}
	if (specular < 0.0)
	{
		specular = 0.0;
	}
	if (shininess < 0.0)
	{
		shininess = 0.0;
	}
}

colour_t material_t::lighting(const Light& light, const tuple_t& position, const tuple_t& eye_vector, const tuple_t& normal_vector) const
{
	// combine the surface color with the light's color/intensity
	colour_t effective_colour{ colour * light.intensity };
	// find the direction to the light source
	tuple_t light_vector{ light.position() - position};
	light_vector.normalize();
	// compute the ambient contribution
	const colour_t ambient_colour{ effective_colour * ambient };
	colour_t diffuse_colour{};
	colour_t specular_colour{};
	// light_dot_normal represents the cosine of the angle between the light vector and the normal vector.
	// A negative number means the light is on the other side of the surface.
	const double light_dot_normal{ tuple_t::dot(light_vector, normal_vector) };
	colour_t black{ 0, 0, 0 };
	if (light_dot_normal < 0)
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

bool material_t::operator==(const material_t& m) const
{
	const double a_comp{ std::fabs(this->ambient - m.ambient) };
	const double d_comp{ std::fabs(this->diffuse - m.diffuse) };
	const double spec_comp{ std::fabs(this->specular - m.specular) };
	const double shine_comp{ std::fabs(this->shininess - m.shininess) };
	if (this->colour == m.colour && a_comp < EPSILON && d_comp < EPSILON && spec_comp < EPSILON && shine_comp < EPSILON)
	{
		return true;
	}
	return false;
}

