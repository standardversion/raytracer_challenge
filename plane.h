#pragma once
#include "ray.h"
#include "matrix.h"
#include "tuple.h"
#include "material.h"
#include "geometry.h"
#include "uv.h"

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
     * @brief Computes the UV coordinates for a point on the plane's surface.
     *
     * Maps a point on a plane face to 2D UV texture coordinates in [0, 1] range.
     *
     * @param point A point on the plane in local space.
     * @return uv_t The corresponding UV coordinates.
     */
    uv_t get_uv(const tuple_t& point) const override;

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
     * @param alpha Barycentric alpha value (unused in flat triangle).
     * @param beta Barycentric beta value (unused in flat triangle).
     * @param gamma Barycentric gamma value (unused in flat triangle).
     * @return The up-facing unit vector (0, 1, 0).
     */
    tuple_t local_normal_at(const tuple_t& local_point, const double alpha = 0, const double beta = 0, const double gamma = 0) const override;

    /**
     * @brief Returns the bounding box of the object.
     *
     * Overrides the base class implementation to provide the specific bounding box
     * for this derived class.
     *
     * @return bbox_t The bounding box of the object.
     */
    bbox_t bounds() const override;
};


