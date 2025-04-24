#include <cmath>
#include "phong.h"
#include "settings.h"

colour_t Phong::lighting
(
	Light& light,
	const Geometry* geo,
	const tuple_t& position,
	const tuple_t& eye_vector,
	const tuple_t& normal_vector,
	const double intensity
) const
{
	colour_t material_coluur{ colour };
	if (pattern)
	{
		material_coluur = pattern->at_object(geo, position);
	}
	// combine the surface color with the light's color/intensity
	colour_t effective_colour{ material_coluur * light.intensity };
	
	// compute the ambient contribution
	const colour_t ambient_colour{ effective_colour * ambient };
	colour_t diffuse_colour{};
	colour_t specular_colour{};
	colour_t black{ 0, 0, 0 };
	colour_t sum{ black };
	const int usteps = light.get_usteps();
	const int vsteps = light.get_vsteps();
	for (int u{ 0 }; u < usteps; u++)
	{
		for (int v{ 0 }; v < vsteps; v++)
		{
			tuple_t point_on_light{ light.point_on_light(u, v) };
			// find the direction to the light source
			tuple_t light_vector{ point_on_light - position };
			light_vector.normalize();
			// light_dot_normal represents the cosine of the angle between the light vector and the normal vector.
			// A negative number means the light is on the other side of the surface.
			const double light_dot_normal{ tuple_t::dot(light_vector, normal_vector) };
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
			sum += diffuse_colour;
			sum += specular_colour;
		}
	}
	// Add the three contributions together to get the final shading
	return ambient_colour + (sum / (usteps * vsteps)) * intensity;
}

bool Phong::operator==(const Material& m) const
{
	// Try to cast to Phong
	const Phong* other = dynamic_cast<const Phong*>(&m);
	if (!other) return false; // Not the same type

	return *this == *other;
}

bool Phong::operator==(const Phong& m) const
{
	return colour == m.colour &&
		std::fabs(ambient - m.ambient) < EPSILON &&
		std::fabs(diffuse - m.diffuse) < EPSILON &&
		std::fabs(specular - m.specular) < EPSILON &&
		std::fabs(shininess - m.shininess) < EPSILON;
}