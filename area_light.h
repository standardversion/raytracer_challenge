#pragma once
#include "light.h"
#include "colour.h"
#include "tuple.h"
#include "ray.h"
#include "sequence.h"


/**
 * @brief Represents a rectangular area light source that emits light across a surface.
 *
 * Unlike a point light, an AreaLight emits light from a defined rectangular region in space,
 * producing softer shadows and more realistic illumination. This light is defined by a corner,
 * two edge vectors (`uvec` and `vvec`), and a jittering sequence for randomized sampling.
 */
class AreaLight : public Light
{
public:
    /**
     * @brief The corner point of the rectangular light surface in world space.
     *
     * This defines the origin (bottom-left corner) of the area from which light is emitted.
     */
    tuple_t corner;

    /**
     * @brief A unit vector representing one edge of the rectangle in the U direction.
     *
     * This vector defines the horizontal direction and size of the area light.
     * The actual full width is divided internally by `usteps` to create sampling points.
     */
    tuple_t uvec;

    /**
     * @brief A unit vector representing one edge of the rectangle in the V direction.
     *
     * This vector defines the vertical direction and size of the area light.
     * Like `uvec`, it is divided by `vsteps` to generate a grid of sample points.
     */
    tuple_t vvec;

    /**
     * @brief A random sequence generator used for jittering sample positions.
     *
     * Helps avoid artifacts by perturbing the (u, v) coordinates slightly to simulate
     * stochastic sampling across the light surface.
     */
    Sequence jitter;

    /**
     * @brief Constructs an AreaLight with a rectangular shape and jitter sampling.
     *
     * @param corner The bottom-left corner of the light rectangle in world space.
     * @param full_uvec The full edge vector in the U direction (width of the light).
     * @param usteps Number of sampling divisions along the U direction.
     * @param full_vvec The full edge vector in the V direction (height of the light).
     * @param vsteps Number of sampling divisions along the V direction.
     * @param jitter A `Sequence` object for jittered sampling.
     * @param intensity The color and brightness of the light (default: white).
     */
    AreaLight(
        const tuple_t& corner,
        const tuple_t& full_uvec,
        const int usteps,
        const tuple_t& full_vvec,
        const int vsteps,
        Sequence& jitter,
        const colour_t& intensity = colour_t{ 1, 1, 1 }
    );

    /**
     * @brief Computes the geometric center of the area light in world space.
     *
     * This is used in some shading models as a representative position of the light.
     *
     * @return A `tuple_t` representing the central point of the light surface.
     */
    tuple_t position() const override;

    /**
     * @brief Returns a point on the light surface based on the given normalized (u, v) coordinates.
     *
     * This function adds jitter to simulate distributed light emission across the area.
     *
     * @param u Normalized horizontal coordinate in the [0, 1] range.
     * @param v Normalized vertical coordinate in the [0, 1] range.
     * @return A `tuple_t` representing a world-space location on the light surface.
     */
    tuple_t point_on_light(const double u, const double v) override;

    /**
     * @brief Computes the light intensity at a given point in the scene.
     *
     * Uses multiple samples across the light surface and accounts for shadows
     * and occlusion in the world to determine how much light reaches the point.
     *
     * @param point The point in world space to evaluate the light's contribution.
     * @param w The world scene used for visibility testing.
     * @return A scalar value in [0.0, 1.0] representing the light's effective intensity.
     */
    double intensity_at(const tuple_t& point, const World& w) override;

    /**
     * @brief Compares this light with another light instance for equivalence.
     *
     * Used for detecting duplicates or testing equality in lighting configuration.
     *
     * @param l The other `Light` object to compare.
     * @return `true` if the lights are functionally equivalent; otherwise, `false`.
     */
    bool operator==(const Light& l) const override;

    /**
     * @brief Compares this light with another `AreaLight` for full equivalence.
     *
     * Includes checks for corner position, vectors, sampling configuration, jitter, and intensity.
     *
     * @param l The other `AreaLight` to compare.
     * @return `true` if all properties match; otherwise, `false`.
     */
    bool operator==(const AreaLight& l) const;
};

