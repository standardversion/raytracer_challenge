#pragma once
#include <utility>
#include "ray.h"
#include "matrix.h"
#include "tuple.h"
#include "material.h"
#include "geometry.h"
#include "uv.h"

struct intersections_t;


/**
 * @class Cylinder
 * @brief Represents an infinite or finite cylinder aligned along the Y-axis.
 *
 * A cylinder can be open or closed, and its height can be bounded by `minimum` and `maximum`.
 * It inherits from Geometry and provides logic for ray intersections and normal calculations.
 */
class Cylinder : public Geometry
{
public:
    /**
     * @brief The minimum Y-value of the cylinder.
     *
     * Rays below this value are considered outside the finite bounds of the cylinder.
     * Default is negative infinity, making the cylinder infinitely long in that direction.
     */
    double minimum{ -INFINITY };

    /**
     * @brief The maximum Y-value of the cylinder.
     *
     * Rays above this value are considered outside the finite bounds of the cylinder.
     * Default is positive infinity, making the cylinder infinitely long in that direction.
     */
    double maximum{ INFINITY };

    /**
     * @brief Whether the cylinder is closed (has flat caps at min and max Y).
     */
    bool closed{ false };

    /**
     * @brief Factory method to create a new Cylinder instance.
     *
     * @return A std::shared_ptr to the newly created Cylinder.
     */
    static std::shared_ptr<Cylinder> create();

    /**
     * @brief Computes the UV coordinates for a point on the cylinder's surface.
     *
     * Maps a point on a cylinder face to 2D UV texture coordinates in [0, 1] range.
     *
     * @param point A point on the cylinder in local space.
     * @return uv_t The corresponding UV coordinates.
     */
    uv_t get_uv(const tuple_t& point) const override;

    /**
     * @brief Computes intersections of a ray with the cylinder in local space.
     *
     * Adds all valid intersection points (including optional caps) to the given collection.
     *
     * @param local_ray The ray in object (local) space.
     * @param intersections A reference to the collection to store intersections.
     */
    void local_intersect(const ray_t& local_ray, intersections_t& intersections) const override;

    /**
     * @brief Computes the surface normal at a given local-space point on the cylinder.
     *
     * Handles curved surface and end-cap normals depending on the point's location.
     *
     * @param local_point The point in the cylinder's local space.
     * @param alpha Barycentric alpha value (unused in flat triangle).
     * @param beta Barycentric beta value (unused in flat triangle).
     * @param gamma Barycentric gamma value (unused in flat triangle).
     * @return A normalized vector representing the surface normal at that point.
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

protected:
    /**
     * @brief Helper function to compute ray intersections with the cylinder's end caps.
     *
     * Called only if the cylinder is closed.
     *
     * @param local_ray The ray in object (local) space.
     * @param intersections A reference to the collection to store intersections.
     */
    void intersect_caps(const ray_t& local_ray, intersections_t& intersections) const;

    /**
     * @brief Checks if a ray intersects a cap at a given time value.
     *
     * Used to determine whether a ray hits one of the cylinder's end caps.
     *
     * @param local_ray The ray in object (local) space.
     * @param time The time (t-value) along the ray being checked.
     * @return True if the ray hits the cap, false otherwise.
     */
    bool check_cap(const ray_t& local_ray, const double time) const;
};
