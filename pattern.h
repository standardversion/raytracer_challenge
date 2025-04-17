#pragma once
#include "colour.h"
#include "tuple.h"
#include "matrix.h"
#include "geometry.h"

/**
 * @struct pattern_t
 * @brief Represents a color pattern applied to a material's surface.
 *
 * A pattern defines how color varies across the surface of a geometry.
 * It can be based on spatial position, enabling effects such as stripes,
 * gradients, checkers, or procedural textures. Patterns support their own
 * transformation matrix to allow independent scaling, rotation, or translation.
 */
struct pattern_t
{
    /**
     * @brief First color used in the pattern.
     *
     * This color typically alternates with `b` depending on the pattern logic.
     */
    colour_t a;

    /**
     * @brief Second color used in the pattern.
     *
     * This color typically alternates with `a` depending on the pattern logic.
     */
    colour_t b;

    /**
     * @brief Transformation matrix applied to the pattern.
     *
     * Allows the pattern to be independently scaled, rotated, or translated
     * relative to the object it is applied to. Default is the identity matrix.
     */
    matrix_t transform{ matrix_t::identity() };

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

    /**
     * @brief Evaluates the pattern color at a point on a geometry object.
     *
     * This function transforms the point from world space into object space
     * (via the geometry), then into pattern space (via the pattern's transform),
     * before calling `at()`.
     *
     * @param geo The geometry object the pattern is applied to.
     * @param point A point in world space.
     * @return colour_t The color at the given point on the object.
     */
    colour_t at_object(const Geometry* geo, const tuple_t& point) const;
};
