#pragma once
#include "colour.h"
#include "pattern.h"

/**
 * @class Checker
 * @brief A pattern that alternates between two colors in a 3D checkerboard layout.
 *
 * The Checker pattern divides 3D space into unit cubes and alternates colors `a` and `b`
 * depending on the sum of the floored coordinates. This creates a checkerboard-like pattern
 * across all three dimensions.
 */
class Checker : public Pattern
{
public:
    int width{ 2 };
    int height{ 2 };
    /**
     * @brief Constructs a Checker pattern with two alternating colors.
     *
     * @param a The primary color used in the pattern.
     * @param b The secondary color used in the pattern.
     */
    Checker(const colour_t& a, const colour_t& b);

    Checker(int width, int height, const colour_t& a, const colour_t& b);

    /**
     * @brief Computes the color at a point in pattern space using 3D checker logic.
     *
     * Returns color `a` or `b` depending on whether the sum of the floored x, y, and z
     * coordinates is even or odd. Produces a 3D checkerboard effect.
     *
     * @param point A point in pattern space.
     * @return colour_t The resulting color at the specified point.
     */
    colour_t at(const tuple_t& point) const override;

    colour_t at(const double u, const double v) const override;
};
