#pragma once
#include <utility>
#include "ray.h"
#include "matrix.h"
#include "tuple.h"
#include "material.h"
#include "cylinder.h"
#include "bounding_box.h"

struct intersections_t;


/**
 * @class Cone
 * @brief Represents a finite or infinite cone aligned along the Y-axis.
 *
 * A cone is a tapering geometric shape that narrows to a point (the apex),
 * with its tip at y=0 and optionally bounded by a minimum and maximum Y range.
 * Inherits from Cylinder for shared functionality like caps.
 */
class Cone : public Cylinder
{
public:
    /**
     * @brief Factory method to create a new Cone instance.
     *
     * @return A std::shared_ptr to the newly created Cone object.
     */
    static std::shared_ptr<Cone> create();

    /**
     * @brief Computes the intersection points between a ray and the cone in local space.
     *
     * Adds valid intersection times (`t` values) to the provided intersection list,
     * considering both the cone's curved surface and caps (if closed).
     *
     * @param local_ray The ray in the cone's local coordinate space.
     * @param intersections The container to which intersection results will be added.
     */
    void local_intersect(const ray_t& local_ray, intersections_t& intersections) const override;

    /**
     * @brief Computes the surface normal vector at a given local-space point on the cone.
     *
     * The normal differs depending on whether the point is on the side surface or a cap.
     *
     * @param local_point The point on the cone, expressed in object-local coordinates.
     * @param alpha Barycentric alpha value (unused in flat triangle).
     * @param beta Barycentric beta value (unused in flat triangle).
     * @param gamma Barycentric gamma value (unused in flat triangle).
     * @return A normalized vector representing the surface normal at the point.
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
     * @brief Helper function to compute ray intersections with the cones's end caps.
     *
     * Called only if the cone is closed.
     *
     * @param local_ray The ray in object (local) space.
     * @param intersections A reference to the collection to store intersections.
     */
    void intersect_caps(const ray_t& local_ray, intersections_t& intersections) const;

    /**
     * @brief Checks if a ray intersects a cap at a given time value.
     *
     * Used to determine whether a ray hits one of the cone's end caps.
     *
     * @param local_ray The ray in object (local) space.
     * @param time The time (t-value) along the ray being checked.
     * @param height The height of the cone.
     * @return True if the ray hits the cap, false otherwise.
     */
    bool check_cap(const ray_t& local_ray, const double time, const double height) const;
};

