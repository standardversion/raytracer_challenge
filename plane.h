#pragma once
#include "ray.h"
#include "matrix.h"
#include "tuple.h"
#include "material.h"
#include "geometry.h"

struct intersections_t;


/**
 * @class Plane
 * @brief Represents an infinite flat surface extending in the X and Z directions.
 *
 * A plane is a simple geometric shape with a constant normal vector. It lies
 * in the XZ plane by default (i.e., y = 0) and is typically used for floors,
 * ceilings, or other large flat surfaces in ray-traced scenes.
 */
class Plane : public Geometry
{
public:
    /**
     * @brief Creates and returns a new Plane instance.
     *
     * This factory method returns a unique pointer to a newly allocated Plane,
     * ensuring proper memory ownership and initialization.
     *
     * @return A std::shared_ptr to a Plane instance.
     */
    static std::shared_ptr<Plane> create();

    /**
     * @brief Computes intersections between a ray and this plane in local space.
     *
     * Overrides the pure virtual method from Geometry. This method checks if
     * the ray is parallel to the plane and, if not, calculates the intersection
     * point in local coordinates.
     *
     * @param local_ray The ray to test, expressed in the plane's local space.
     * @param intersections The container to which any found intersections will be added.
     */
    void local_intersect(const ray_t& local_ray, intersections_t& intersections) const override;

    /**
     * @brief Calculates the normal vector at a point on the plane in local space.
     *
     * Since a plane has a constant normal, this function always returns (0, 1, 0),
     * regardless of the input point.
     *
     * @param local_point The point on the plane (ignored).
     * @return The up-facing unit vector (0, 1, 0).
     */
    tuple_t local_normal_at(const tuple_t& local_point, const double alpha = 0, const double beta = 0, const double gamma = 0) const override;
};


