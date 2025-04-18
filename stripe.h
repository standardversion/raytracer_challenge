#pragma once
#include "colour.h"
#include "pattern.h"


/**
 * @class Stripe
 * @brief Represents a color pattern applied to a material's surface.
 *
 * A pattern defines how color varies across the surface of a geometry.
 * It can be based on spatial position, enabling effects such as stripes,
 * gradients, checkers, or procedural textures. Patterns support their own
 * transformation matrix to allow independent scaling, rotation, or translation.
 */
class Stripe : public Pattern
{
public:
    Stripe(const colour_t& a, const colour_t& b);
    /**
     * @brief Evaluates the pattern color at a specific point in pattern space.
     *
     * This function implements the core logic of the pattern (e.g., stripes,
     * checkers) and returns either `a`, `b`, or an interpolated color based
     * on the point's coordinates.
     *
     * @param point A point in pattern space.
     * @return colour_t The color at the given point.
     */
    colour_t at(const tuple_t& point) const;
};

