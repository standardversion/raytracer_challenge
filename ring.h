#pragma once
#include "colour.h"
#include "pattern.h"

/**
 * @class Ring
 * @brief A pattern that creates concentric rings alternating between two colors.
 *
 * The Ring pattern alternates between color `a` and color `b` in concentric circles
 * on the X-Z plane, centered at the origin. The pattern is based on the distance from
 * the origin in the X-Z plane, creating a ripple or bullseye-like effect.
 */
class Ring : public Pattern
{
public:
    /**
     * @brief Constructs a Ring pattern with two alternating colors.
     *
     * @param a The primary color used in the rings.
     * @param b The secondary color used in the rings.
     */
    Ring(const colour_t& a, const colour_t& b);

    /**
     * @brief Computes the color at a point in pattern space using radial ring logic.
     *
     * Determines the distance from the origin in the X-Z plane and alternates between
     * color `a` and `b` based on whether the floored radial distance is even or odd.
     *
     * @param point A point in pattern space.
     * @return colour_t The resulting color at the specified point.
     */
    colour_t at(const tuple_t& point) const override;
};
