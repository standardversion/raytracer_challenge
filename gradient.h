#pragma once
#include "colour.h"
#include "pattern.h"

/**
 * @class Gradient
 * @brief A pattern that linearly interpolates between two colors along the x-axis.
 *
 * The Gradient pattern transitions smoothly from color `a` to color `b` across the x-axis.
 * The interpolation is based on the fractional part of the x-coordinate, creating a horizontal
 * gradient effect that repeats every unit.
 */
class Gradient : public Pattern
{
public:
    /**
     * @brief Constructs a Gradient pattern with two endpoint colors.
     *
     * @param a The starting color of the gradient.
     * @param b The ending color of the gradient.
     */
    Gradient(const colour_t& a, const colour_t& b);

    /**
     * @brief Computes the interpolated color at a point in pattern space.
     *
     * Uses the fractional part of the x-coordinate to linearly interpolate between
     * color `a` and color `b`. The result is a repeating horizontal gradient.
     *
     * @param point A point in pattern space.
     * @return colour_t The interpolated color at the specified point.
     */
    colour_t at(const tuple_t& point) const override;
};


