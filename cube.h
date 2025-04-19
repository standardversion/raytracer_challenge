#pragma once
#include <utility>
#include "ray.h"
#include "matrix.h"
#include "tuple.h"
#include "material.h"
#include "geometry.h"

struct intersections_t;

/**
 * @class Cube
 * @brief Represents an axis-aligned unit cube centered at the origin.
 *
 * Inherits from Geometry and provides implementations for ray intersection
 * and surface normal calculation specific to cubes.
 */
class Cube : public Geometry
{
public:

    /**
     * @brief Factory method to create a new Cube instance.
     *
     * @return A std::unique_ptr pointing to a new Cube.
     */
    static std::unique_ptr<Cube> create();

    /**
     * @brief Intersects a ray with the cube in local object space.
     *
     * Determines the points where a ray intersects the cube, if any,
     * and adds them to the given intersections collection.
     *
     * @param local_ray The ray in the cube's local coordinate space.
     * @param intersections A reference to the intersections collection to populate.
     */
    void local_intersect(const ray_t& local_ray, intersections_t& intersections) const override;

    /**
     * @brief Computes the surface normal vector at a given point on the cube.
     *
     * Assumes the point is in local object space and returns a normal vector
     * that is perpendicular to the cube’s face at that point.
     *
     * @param local_point The point on the cube in local space.
     * @return A unit vector (tuple_t) representing the surface normal.
     */
    tuple_t local_normal_at(const tuple_t& local_point) const override;

private:
    /**
     * @brief Constructs a Cube.
     *
     * Private to force use of the static `create()` method for construction.
     */
    explicit Cube();

    /**
     * @brief Helper method to compute intersection range along one axis.
     *
     * Determines the minimum and maximum t-values for where a ray intersects
     * the cube along a single axis (x, y, or z).
     *
     * @param origin The ray origin coordinate along the axis.
     * @param direction The ray direction along the axis.
     * @return A std::pair containing the (tmin, tmax) values for that axis.
     */
    std::pair<double, double> check_axis(const double origin, const double direction) const;
};