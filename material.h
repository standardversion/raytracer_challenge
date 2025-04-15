#pragma once
#include "colour.h"
#include "light.h"
#include "tuple.h"


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
     * @brief Calculates the color of the material when illuminated by a light source.
     *
     * This pure virtual function must be implemented by any subclass. It defines how
     * the material responds to a given light source at a certain position with specific
     * viewing and surface normal directions.
     *
     * @param light The light source affecting the material.
     * @param position The point on the surface being illuminated.
     * @param eye_vector The direction from the point to the eye (viewer).
     * @param normal_vector The surface normal at the point.
     * @return colour_t The resulting color after lighting is applied.
     */
    virtual colour_t lighting(const Light& light, const tuple_t& position, const tuple_t& eye_vector, const tuple_t& normal_vector) const = 0;

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
