#pragma once
#include "pattern.h"
#include "colour.h"

/**
 * @class AlignCheck
 * @brief A pattern used for alignment checking based on color values at different corners.
 *
 * The AlignCheck class inherits from the Pattern base class and defines a specific pattern
 * that can be used to verify alignment by comparing colors at various positions.
 */
class AlignCheck : public Pattern
{
public:
    /**
     * @brief Color at the center or main point.
     */
    colour_t c;

    /**
     * @brief Color at the upper-left corner.
     */
    colour_t d;

    /**
     * @brief Color at the upper-right corner.
     */
    colour_t e;

    /**
     * @brief Constructs an AlignCheck pattern using specified corner colors.
     *
     * @param main Color at the center of the pattern.
     * @param ul Color at the upper-left corner.
     * @param ur Color at the upper-right corner.
     * @param bl Color at the bottom-left corner.
     * @param br Color at the bottom-right corner.
     */
    AlignCheck(const colour_t& main, const colour_t& ul, const colour_t& ur, const colour_t& bl, const colour_t& br);

    /**
     * @brief Returns the color at a given point in 3D space.
     *
     * @param point A 3D point represented as a tuple.
     * @return The color at the specified point.
     */
    colour_t at(const tuple_t& point) const override;

    /**
     * @brief Returns the color at the given UV coordinates.
     *
     * @param u The horizontal coordinate.
     * @param v The vertical coordinate.
     * @return The color at the specified UV position.
     */
    colour_t at(const double u, const double v) const override;
};
