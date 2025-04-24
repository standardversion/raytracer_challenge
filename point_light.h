#pragma once
#include "light.h"
#include "colour.h"
#include "tuple.h"
#include "ray.h"

/**
 * @class PointLight
 * @brief Represents a point light source that emits light uniformly in all directions from a single point.
 *
 * A `PointLight` is a common type of light source used in ray tracing. It has a fixed position in space
 * and does not have an area or direction. The intensity of light received at a point is typically
 * determined by shadow checks and distance falloff (if implemented).
 */
class PointLight : public Light
{
public:
    /**
     * @brief Constructs a new PointLight with the specified intensity.
     *
     * @param intensity The color and brightness of the light. Defaults to white (1, 1, 1).
     */
    PointLight(const colour_t& intensity = colour_t{ 1, 1, 1 });

    /**
     * @brief Computes the light's effective intensity at a specific point in the world.
     *
     * @param point The point in world space to evaluate.
     * @param w The scene world, used for determining shadows or occlusion.
     * @return A value in [0.0, 1.0] representing the light's contribution at the given point.
     *
     * Typically returns full intensity (1.0) if the point is not in shadow. Shadow checks are
     * delegated to the World class.
     */
    double intensity_at(const tuple_t& point, const World& w) override;

    /**
     * @brief Returns a point on the light surface.
     *
     * @param u Ignored.
     * @param v Ignored.
     * @return A `tuple_t` representing a world-space location on the light surface.
     */
    tuple_t point_on_light(const double u, const double v) override;

    /**
     * @brief Compares this PointLight with another Light.
     *
     * @param l A reference to another Light instance.
     * @return true if both lights are PointLights with the same position and intensity; false otherwise.
     *
     * This overrides the virtual comparison in the base class. It returns false if the other light
     * is not a PointLight, or if its internal properties differ.
     */
    bool operator==(const Light& l) const override;

    /**
     * @brief Compares this PointLight with another PointLight.
     *
     * @param l Another PointLight instance.
     * @return true if both lights have the same position and intensity.
     *
     * Provides a type-safe and explicit comparison for two PointLights, useful in testing or scene validation.
     */
    bool operator==(const PointLight& l) const;
};
