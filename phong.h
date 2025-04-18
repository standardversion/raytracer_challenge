#pragma once
#include <memory>
#include "material.h"
#include "pattern.h"

/**
 * @class Phong
 * @brief A concrete implementation of the Material class using the Phong reflection model.
 *
 * This class implements the Phong shading model, which combines ambient, diffuse,
 * and specular lighting components to produce realistic surface illumination.
 */
class Phong : public Material
{
public:
    /**
     * @brief The base color of the material.
     *
     * This color is used in the lighting calculation as the base reflectance of the surface.
     * Default is white (1, 1, 1).
     */
    colour_t colour{ 1, 1, 1 };

    /**
     * @brief Optional pattern applied to the material surface.
     *
     * If set, this pattern overrides the base color and allows for more complex
     * surface detail such as stripes, checkers, gradients, or procedural textures.
     * Patterns are evaluated based on the surface position and geometry.
     */
    std::shared_ptr<Pattern> pattern;

    /**
     * @brief Ambient reflectivity coefficient.
     *
     * Controls how much ambient light the material reflects.
     * Default is 0.1.
     */
    double ambient{ 0.1 };

    /**
     * @brief Diffuse reflectivity coefficient.
     *
     * Controls how much light is reflected diffusely from the surface.
     * Default is 0.9.
     */
    double diffuse{ 0.9 };

    /**
     * @brief Specular reflectivity coefficient.
     *
     * Controls how much light is reflected as a shiny highlight.
     * Default is 0.9.
     */
    double specular{ 0.9 };

    /**
     * @brief Shininess factor for the specular highlight.
     *
     * Determines the sharpness of the specular reflection.
     * Higher values produce smaller, sharper highlights.
     * Default is 200.0.
     */
    double shininess{ 200.0 };

    double reflective{ 0.0 };

    double transparency{ 0.0 };

    double refractive_index{ 1.0 };

    /**
     * @brief Default constructor.
     *
     * Initializes a Phong material with default lighting parameters.
     */
    Phong() = default;

    /**
     * @brief Computes the color at a given point using the Phong lighting model.
     *
     * This method combines the ambient, diffuse, and specular components
     * to simulate realistic lighting on the material. If a pattern is present,
     * it is used instead of the base color during shading.
     *
     * @param light The light source affecting the material.
     * @param geo Pointer to the geometry this material is applied to.
     * @param position The point on the surface being illuminated.
     * @param eye_vector The direction from the point to the viewer.
     * @param normal_vector The normal vector at the surface point.
     * @param in_shadow Whether the point is in shadow (true) or not (false).
     * @return colour_t The resulting color after applying lighting effects.
     */
    virtual colour_t lighting
    (
        const Light& light,
        const Geometry* geo,
        const tuple_t& position,
        const tuple_t& eye_vector,
        const tuple_t& normal_vector,
        const bool in_shadow
    ) const override;

    /**
     * @brief Checks equality between this Phong material and another Material.
     *
     * This method compares the material properties to determine if they are equivalent.
     *
     * @param m The other material to compare with.
     * @return true If the materials are equal.
     * @return false Otherwise.
     */
    virtual bool operator==(const Material& m) const override;
};
