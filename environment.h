#pragma once
#include "tuple.h"

/**
 * @brief Represents the physical environment in which objects move.
 *
 * Typically used in physics simulations to model the effects of gravity and wind
 * on projectiles or particles in a scene.
 */
struct environment_t
{
    /**
     * @brief The gravitational force vector applied to objects in the environment.
     *
     * Usually points downward (e.g., (0, -9.8, 0)) to simulate Earth's gravity.
     */
    tuple_t gravity{ tuple_t::vector(0, 0, 0) };

    /**
     * @brief The wind force vector applied to objects in the environment.
     *
     * Can be used to simulate directional air resistance or drag.
     */
    tuple_t wind{ tuple_t::vector(0, 0, 0) };
};

