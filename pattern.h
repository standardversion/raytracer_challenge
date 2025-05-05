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
 * Derived classes implement specific behaviors such as stripes, gradients, checkers, or image-based UV maps.
 * Each pattern has its own transformation matrix, allowing independent scaling, rotation, or translation
 * relative to the object it is applied to.
 */
class Pattern
{
public:
    /**
     * @brief Primary color of the pattern.
     *
     * Often alternates or blends with `b` depending on the specific pattern logic.
     */
    colour_t a;

    /**
     * @brief Secondary color of the pattern.
     *
     * Used alongside `a` to define variation across the surface—e.g., in alternating or gradient patterns.
     */
    colour_t b;

    /**
     * @brief Transformation matrix for the pattern.
     *
     * Transforms the input point into pattern space independently from the object.
     * Defaults to the identity matrix (no transformation).
     */
    matrix_t transform{ matrix_t::identity() };

    /**
     * @brief Computes the pattern color at a given point in pattern space.
     *
     * Pure virtual function. Derived classes must implement this to define how the pattern
     * generates color based on spatial coordinates.
     *
     * @param point A point in pattern-local coordinates (after transforms).
     * @return colour_t The resulting color at the given point.
     */
    virtual colour_t at(const tuple_t& point) const = 0;

    /**
     * @brief Computes the pattern color using 2D UV coordinates.
     *
     * Default implementation returns `a`. Override this method in patterns that support UV mapping.
     *
     * @param u Horizontal texture coordinate, typically in [0, 1].
     * @param v Vertical texture coordinate, typically in [0, 1].
     * @return colour_t The color at the specified UV coordinate.
     */
    virtual colour_t at(const double u, const double v) const;

    /**
     * @brief Indicates whether the pattern supports UV coordinate mapping.
     *
     * Default implementation returns false. UV-aware patterns (e.g. image textures) should override this.
     *
     * @return true if the pattern uses UV coordinates; false otherwise.
     */
    virtual bool supports_uv() const;

    /**
     * @brief Computes the pattern color at a point on a geometry object.
     *
     * Transforms the input world-space point to object-local space using the geometry's transform,
     * then to pattern space using the pattern's transform. Calls `at()` to evaluate the final color.
     *
     * @param geo Pointer to the geometry object the pattern is applied to.
     * @param point A point in world space.
     * @return colour_t The resulting pattern color at the specified point.
     */
    virtual colour_t at_object(const Geometry* geo, const tuple_t& point) const;

protected:
    /**
     * @brief Constructs a pattern with specified primary and secondary colors.
     *
     * @param a The primary color.
     * @param b The secondary color.
     */
    Pattern(const colour_t& a, const colour_t& b);

    /**
     * @brief Constructs a pattern with default color values.
     *
     * Defaults may be overridden in derived pattern constructors.
     */
    Pattern();
};