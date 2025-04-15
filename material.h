#pragma once
#include "colour.h"
#include "light.h"
#include "tuple.h"

/**
 * @brief Describes the surface properties of a geometric object for lighting calculations.
 *
 * A material defines how an object interacts with light, including how it reflects
 * diffuse and specular components, and its overall color.
 */
struct material_t
{
	/**
	 * @brief The base colour of the material.
	 *
	 * Used in combination with light sources to compute the final colour at a point on the surface.
	 */
	colour_t colour{ 1, 1, 1 };

	/**
	 * @brief The ambient reflection coefficient.
	 *
	 * Controls how much ambient light the surface reflects (i.e. light present everywhere).
	 * Typical range is [0.0, 1.0].
	 */
	double ambient{ 0.1 };

	/**
	 * @brief The diffuse reflection coefficient.
	 *
	 * Determines how much light is reflected in all directions from a surface.
	 * Affects how "matte" the surface appears. Range: [0.0, 1.0].
	 */
	double diffuse{ 0.9 };

	/**
	 * @brief The specular reflection coefficient.
	 *
	 * Controls the brightness of specular highlights (the shiny spots).
	 * Range: [0.0, 1.0].
	 */
	double specular{ 0.9 };

	/**
	 * @brief The shininess factor used in specular highlight calculation.
	 *
	 * Higher values produce smaller, tighter highlights, simulating a smoother surface.
	 * Typical values range from 10 to 300+.
	 */
	double shininess{ 200.0 };

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

	/**
	 * @brief Calculates the color of the material under lighting at a given point.
	 *
	 * @param light The light source affecting the material.
	 * @param position The position on the surface where the light is being evaluated.
	 * @param eye_vector The direction vector from the surface point to the eye/camera.
	 * @param normal_vector The surface normal vector at the point of contact.
	 * @return colour_t The resulting color after applying the lighting model.
	 */
	colour_t lighting(const Light& light, const tuple_t& position, const tuple_t& eye_vector, const tuple_t& normal_vector) const;

	/**
	 * @brief Compares two materials for equality.
	 *
	 * @param t The material to compare against.
	 * @return true if all properties are equal; false otherwise.
	 */
	bool operator==(const material_t& t) const;
};