#pragma once
#include "scene_object.h"
#include "colour.h"
#include "tuple.h"
#include "ray.h"

/**
 * @brief Represents a point light source in the scene.
 *
 * A light emits illumination from a position in space and has an intensity (color and brightness).
 * Inherits from SceneObject to allow transformations (e.g., moving or rotating lights).
 */
class Light : public SceneObject
{
public:
    /**
     * @brief The intensity (color and brightness) of the light.
     *
     * Typically a colour_t value where RGB channels are in the [0.0, 1.0] range.
     */
    colour_t intensity{};

    /**
     * @brief Constructs a new Light with the given intensity.
     *
     * @param intensity The color and brightness of the light. Default is white light (1,1,1).
     */
    Light(const colour_t& intensity = colour_t{ 1,1,1 });

    /**
     * @brief Gets the position of the light in world space.
     *
     * The position is derived from the SceneObject's transformation matrix.
     * @return A tuple_t representing the world-space location of the light.
     */
    tuple_t position() const;
};