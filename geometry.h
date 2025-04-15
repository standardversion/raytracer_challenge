#pragma once
#include <memory>
#include "scene_object.h"
#include "material.h"
#include "ray.h"

struct intersections_t;

/**
 * @brief Abstract base class for all geometric shapes in the scene.
 *
 * Geometry represents objects that can be intersected by rays and can have materials.
 * All shapes inherit from this and must implement `local_normal_at()` and `intersect()`.
 */
class Geometry : public SceneObject
{
public:
    /**
     * @brief The material associated with this geometry.
     *
     * Determines the surface properties (color, reflectivity, etc.).
     */
    std::shared_ptr<material_t> material{ std::make_shared<material_t>() };

    /**
     * @brief Calculates the normal vector at a given point in the object's local space.
     *
     * This is a pure virtual method that must be implemented by derived classes
     * to provide shape-specific normal calculations.
     *
     * @param local_point The point in object-local space to compute the normal at.
     * @return tuple_t The normal vector at the point (unit vector).
     */
    virtual tuple_t local_normal_at(const tuple_t& local_point) const = 0;

    /**
     * @brief Computes intersections between a ray and this geometry.
     *
     * Derived classes must implement this to calculate intersections in object-local space.
     *
     * @param ray The ray to test for intersection.
     * @param intersections The container to add any resulting intersections to.
     */
    virtual void intersect(const ray_t& ray, intersections_t& intersections) const = 0;

    /**
     * @brief Calculates the world-space normal at a given point on the surface.
     *
     * Transforms the point to local space, computes the local normal, and then
     * transforms it back to world space.
     *
     * @param point The point on the geometry in world space.
     * @return tuple_t The normal vector at the point (unit vector).
     */
    tuple_t normal_at(const tuple_t& point) const;

protected:
    /**
     * @brief Protected constructor to ensure Geometry is abstract.
     */
    Geometry();
};

