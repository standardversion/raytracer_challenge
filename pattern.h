#pragma once
#include "colour.h"
#include "tuple.h"
#include "matrix.h"
#include "geometry.h"

/**
 * @class Pattern
 * @brief Abstract base class representing a color pattern applied to a geometry's surface.
 *
 * A pattern defines how colors vary across the surface of a geometry using spatial logic.
 * Derived classes implement specific pattern behaviors such as stripes, gradients, or checkers.
 * Each pattern maintains its own transformation matrix, enabling independent scaling, rotation,
 * or translation relative to the geometry it is applied to.
 */
class Pattern
{
public:
    /**
     * @brief Primary color of the pattern.
     *
     * Often alternates with `b` depending on the specific pattern implementation.
     */
    colour_t a;

    /**
     * @brief Secondary color of the pattern.
     *
     * Used alongside `a` in alternating or interpolated logic to produce the final effect.
     */
    colour_t b;

    /**
     * @brief Transformation matrix for the pattern.
     *
     * Transforms pattern space independently of the object to which it is applied.
     * Defaults to the identity matrix, meaning no transformation.
     */
    matrix_t transform{ matrix_t::identity() };

    /**
     * @brief Computes the pattern color at a point in pattern space.
     *
     * This is a pure virtual method to be implemented by derived classes.
     * It defines the core pattern logic using the input point's coordinates
     * to return either `a`, `b`, or a color derived from them.
     *
     * @param point A point expressed in pattern space coordinates.
     * @return colour_t The resulting color at the specified point.
     */
    virtual colour_t at(const tuple_t& point) const = 0;

    virtual colour_t at(const double u, const double v) const;

    virtual bool supports_uv() const;

    /**
     * @brief Computes the pattern color at a point on a geometry object.
     *
     * This method transforms a point from world space to object space using
     * the geometry's transform, and then from object space to pattern space
     * using the pattern's own transform. The final color is computed by calling `at()`.
     *
     * @param geo Pointer to the geometry object to which the pattern is applied.
     * @param point A point in world space.
     * @return colour_t The resulting color at the point on the geometry.
     */
    virtual colour_t at_object(const Geometry* geo, const tuple_t& point) const;

protected:
    /**
     * @brief Constructs a Pattern with specified primary and secondary colors.
     *
     * @param a The primary color.
     * @param b The secondary color.
     */
    Pattern(const colour_t& a, const colour_t& b);

    /**
     * @brief Constructs a Pattern with default colors.
     *
     * Defaults may vary based on derived class behavior or internal logic.
     */
    Pattern();
};
