#pragma once
#include "tuple.h"

/**
 * @brief Represents a moving object affected by forces like gravity and wind.
 *
 * Commonly used in basic physics simulations to model projectile motion
 * over time under the influence of an environment.
 */
struct projectile_t
{
    /**
     * @brief The current position of the projectile in world space.
     *
     * Typically updated every frame or timestep based on velocity and forces.
     */
    tuple_t position{ tuple_t::point(0, 0, 0) };

    /**
     * @brief The current velocity vector of the projectile.
     *
     * Determines the direction and speed of the projectile's motion.
     * Affected by environmental forces like gravity and wind.
     */
    tuple_t velocity{ tuple_t::vector(0, 0, 0) };
};
