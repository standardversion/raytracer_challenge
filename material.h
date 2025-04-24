#pragma once
#include "colour.h"
#include "light.h"
#include "tuple.h"

class Geometry;

/**
 * @class Material
 * @brief Abstract base class representing a material in a rendering engine.
 *
 * This class provides a common interface for all material types used in lighting calculations.
 * It defines how light interacts with a surface through the pure virtual method `lighting`,
 * which must be implemented by derived classes.
 *
 * @note This class is intended to be used as a base class and cannot be instantiated directly.
 */
class Material
{
protected:
    /**
     * @brief Protected default constructor.
     *
     * Ensures that Material cannot be instantiated directly,
     * but only through derived classes.
     */
    Material() = default;

public:
    /**
     * @brief Calculates the color of the material under a specific lighting condition.
     *
     * This pure virtual function must be implemented by any subclass to define how the
     * material reacts to illumination. It determines the final color at a surface point
     * given the lighting conditions, viewer perspective, surface geometry, and shadowing.
     *
     * @param light The light source affecting the material.
     * @param geo Pointer to the geometry object the material is applied to. This can be
     *        used to access material-specific properties or additional context.
     * @param position The point on the surface where the lighting calculation is performed.
     * @param eye_vector The direction from the surface point toward the eye (camera/viewer).
     * @param normal_vector The surface normal at the point of intersection.
     * @param intensity Light intensity at point.
     * @return colour_t The final color resulting from the lighting model applied at the given point.
     */
    virtual colour_t lighting
    (
        const Light& light,
        const Geometry* geo,
        const tuple_t& position,
        const tuple_t& eye_vector,
        const tuple_t& normal_vector,
        const double intensity
    ) const = 0;


    /**
     * @brief Compares this material with another for equality.
     *
     * This pure virtual operator should be overridden to provide material-specific equality checks.
     *
     * @param m The other material to compare with.
     * @return true If the materials are considered equal.
     * @return false Otherwise.
     */
    virtual bool operator==(const Material& m) const = 0;
};
