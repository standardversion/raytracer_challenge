#pragma once
#include <utility>
#include "ray.h"
#include "matrix.h"
#include "tuple.h"
#include "material.h"
#include "cylinder.h"

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
     * @return A std::unique_ptr to the newly created Cone object.
     */
    static std::unique_ptr<Cone> create();

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
     * @return A normalized vector representing the surface normal at the point.
     */
    tuple_t local_normal_at(const tuple_t& local_point) const override;

private:
    /**
     * @brief Constructs a Cone object.
     *
     * This constructor is private to enforce controlled creation through the `create()` method.
     */
    explicit Cone();
};

