#pragma once
#include "scene_object.h"
#include "colour.h"
#include "tuple.h"
#include "ray.h"

class World;
/**
 * @brief Represents a point light source in the scene.
 *
 * A light emits illumination from a specific position in space and has an associated intensity,
 * typically defined by color and brightness. This base class supports transformation via
 * SceneObject, allowing lights to be positioned or oriented within the scene.
 */
class Light : public SceneObject
{
public:
    /**
     * @brief The intensity (color and brightness) of the light.
     *
     * Typically a `colour_t` value where RGB channels are in the [0.0, 1.0] range.
     * Intensity influences how strongly the light affects objects in the scene.
     */
    colour_t intensity{};

    /**
     * @brief Constructs a new Light with the given intensity.
     *
     * @param intensity The color and brightness of the light.
     *                  Defaults to white light (1, 1, 1).
     */
    Light(const colour_t& intensity = colour_t{ 1, 1, 1 });

    /**
     * @brief Gets the position of the light in world space.
     *
     * The position is calculated using the transformation matrix inherited from SceneObject.
     * By default, this assumes the light's local-space origin is at (0, 0, 0).
     *
     * @return A `tuple_t` representing the world-space location of the light.
     */
    virtual tuple_t position() const;

    /**
     * @brief Determines if this light is equal to another light.
     *
     * @param l The other `Light` instance to compare against.
     * @return `true` if both lights share equivalent position, intensity, and other relevant state; `false` otherwise.
     *
     * This method is pure virtual and must be implemented by derived light types.
     * It is commonly used for testing, deduplication, or scene configuration validation.
     */
    virtual bool operator==(const Light& l) const = 0;

    /**
     * @brief Computes the effective intensity of the light at a specific point in the world.
     *
     * This method may account for occlusion, attenuation, or other environmental effects
     * depending on the specific light type. For a basic point light, this typically returns
     * full intensity if the point is not in shadow.
     *
     * @param point The point in world space to evaluate the light's contribution at.
     * @param w The world context, used for shadow checks and scene information.
     * @return A scalar value (typically in [0.0, 1.0]) representing the light's effective strength at the point.
     */
    virtual double intensity_at(const tuple_t& point, const World& w) const = 0;
};