#pragma once
#include "colour.h"
#include "light.h"
#include "tuple.h"

struct material_t
{
	colour_t colour{ 1, 1, 1 };
	double ambient{ 0.1 };
	double diffuse{ 0.9 };
	double specular{ 0.9 };
	double shininess{ 200.0 };

	// CONSTRUCTOR

	/**
	 * @brief Constructs a new material with optional properties.
	 *
	 * @param c The base color of the material (default is white: (1, 1, 1)).
	 * @param a The ambient reflectivity (default is 0.1).
	 * @param d The diffuse reflectivity (default is 0.9).
	 * @param spec The specular reflectivity (default is 0.9).
	 * @param shine The shininess factor for specular highlights (default is 200.0).
	 */
	material_t(
		const colour_t c = colour_t(1, 1, 1),
		const double a = 0.1,
		const double d = 0.9,
		const double spec = 0.9,
		const double shine = 200.0
	);

	// MEMBER FUNCTIONS
	/**
	 * @brief Calculates the color of the material under lighting at a given point.
	 *
	 * @param light The light source affecting the material.
	 * @param position The position on the surface where the light is being evaluated.
	 * @param eye_vector The direction vector from the surface point to the eye/camera.
	 * @param normal_vector The surface normal vector at the point of contact.
	 * @return colour_t The resulting color after applying the lighting model.
	 */
	colour_t lighting(const light_t& light, const tuple_t& position, const tuple_t& eye_vector, const tuple_t& normal_vector) const;

	// OPERATORS
	/**
	 * @brief Compares two materials for equality.
	 *
	 * @param t The material to compare against.
	 * @return true if all properties are equal; false otherwise.
	 */
	bool operator==(const material_t& t) const;
};