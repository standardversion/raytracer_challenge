#pragma once
#include "colour.h"
#include "pattern.h"

/**
 * @class Checker
 * @brief A pattern that alternates between two colors in a 3D checkerboard layout.
 *
 * The Checker pattern divides 3D space into a grid and alternates colors `a` and `b`
 * depending on the coordinates. This creates a checkerboard-like pattern that can be
 * customized in width and height for 2D applications, and extends to 3D using floored coordinates.
 */
class Checker : public Pattern
{
public:
    /**
     * @brief Width of the checker cells in UV mapping.
     */
    int width{ 2 };

    /**
     * @brief Height of the checker cells in UV mapping.
     */
    int height{ 2 };

    /**
     * @brief Constructs a 3D Checker pattern with two alternating colors.
     *
     * This constructor uses default width and height for UV mapping.
     *
     * @param a The primary color used in the pattern.
     * @param b The secondary color used in the pattern.
     */
    Checker(const colour_t& a, const colour_t& b);

    /**
     * @brief Constructs a Checker pattern with specified UV dimensions and two colors.
     *
     * Allows customization of the checker cell size in UV space.
     *
     * @param width Number of divisions along the U axis.
     * @param height Number of divisions along the V axis.
     * @param a The primary color used in the pattern.
     * @param b The secondary color used in the pattern.
     */
    Checker(int width, int height, const colour_t& a, const colour_t& b);

    /**
     * @brief Computes the color at a point in 3D space using checker logic.
     *
     * Determines color `a` or `b` based on the sum of the floored x, y, and z coordinates.
     *
     * @param point A point in pattern (3D object) space.
     * @return The resulting color at the specified point.
     */
    colour_t at(const tuple_t& point) const override;

    /**
     * @brief Computes the color at a 2D UV coordinate using checker logic.
     *
     * Alternates colors based on the current cell defined by UV coordinates
     * and the configured width and height.
     *
     * @param u The horizontal texture coordinate.
     * @param v The vertical texture coordinate.
     * @return The resulting color at the specified UV coordinate.
     */
    colour_t at(const double u, const double v) const override;
};
