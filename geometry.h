#pragma once
#include <memory>
#include "scene_object.h"
#include "material.h"
#include "ray.h"
#include "uv.h"
#include "bounding_box.h"

struct intersections_t;

/**
 * @class Geometry
 * @brief Abstract base class for all geometric shapes in the scene.
 *
 * Geometry represents any object that can be intersected by rays and may have
 * materials and UV mappings. All derived shapes must implement their own logic
 * for intersection, surface normals, and bounding volumes.
 */
class Geometry : public SceneObject
{
public:
    /**
     * @brief Determines whether the geometry casts shadows.
     */
    bool cast_shadows{ true };

    /**
     * @brief Indicates if the shape supports UV mapping.
     */
    bool has_uvs{ false };

    /**
     * @brief The material associated with this geometry.
     *
     * Defines the visual and reflective properties of the shape.
     */
    std::shared_ptr<Material> material;

    /**
     * @brief Calculates the surface normal in local space at a given point.
     *
     * Pure virtual method that must be implemented by derived classes.
     * Used for lighting calculations and reflections.
     *
     * @param local_point The point on the object in its local space.
     * @param alpha Barycentric alpha value (used for triangle smoothing).
     * @param beta Barycentric beta value (used for triangle smoothing).
     * @param gamma Barycentric gamma value (used for triangle smoothing).
     * @return tuple_t The normalized surface normal at the specified point.
     */
    virtual tuple_t local_normal_at(const tuple_t& local_point, const double alpha = 0, const double beta = 0, const double gamma = 0) const = 0;

    /**
     * @brief Calculates intersections between a ray and the object in local space.
     *
     * Pure virtual method to be implemented by each shape class. It computes where
     * the ray intersects the shape, if at all.
     *
     * @param local_ray The ray expressed in the object's local coordinate space.
     * @param intersections The collection to append resulting intersection data.
     */
    virtual void local_intersect(const ray_t& local_ray, intersections_t& intersections) const = 0;

    /**
     * @brief Computes intersections between a ray and this geometry.
     *
     * Applies the object’s transformation to the ray and delegates to the local
     * intersection logic. This is the method typically called by the scene or engine.
     *
     * @param ray The incoming ray in world coordinates.
     * @param intersections The container where intersections will be stored.
     */
    void intersect(const ray_t& ray, intersections_t& intersections) const;

    /**
     * @brief Computes the surface normal at a point in world space.
     *
     * Internally converts the point to local space, computes the local normal,
     * and transforms it back into world space, accounting for non-uniform scaling.
     *
     * @param point The point on the object in world space.
     * @param alpha Barycentric alpha value (used for triangle smoothing).
     * @param beta Barycentric beta value (used for triangle smoothing).
     * @param gamma Barycentric gamma value (used for triangle smoothing).
     * @return tuple_t The normal vector in world space.
     */
    tuple_t normal_at(const tuple_t& point, const double alpha = 0, const double beta = 0, const double gamma = 0) const;

    /**
     * @brief Computes the bounding box of the shape in parent (world) space.
     *
     * Transforms the shape's local bounding box using its transformation matrix
     * and returns the resulting axis-aligned bounding box.
     *
     * @return bbox_t The bounding box in world or parent space.
     */
    bbox_t bounds_in_parent_space() const;

    /**
     * @brief Returns the bounding box of the shape in local space.
     *
     * Pure virtual method to be implemented by all geometry types.
     *
     * @return bbox_t The axis-aligned bounding box in local object space.
     */
    virtual bbox_t bounds() const = 0;

    /**
     * @brief Retrieves the UV coordinates for a given point on the object.
     *
     * Default implementation returns (0, 0). Override this in derived classes
     * that support UV mapping.
     *
     * @param point A point on the object in local space.
     * @return uv_t The UV texture coordinates at the given point.
     */
    virtual uv_t get_uv(const tuple_t& point) const;

protected:
    /**
     * @brief Protected constructor to ensure Geometry is abstract.
     *
     * Only derived classes can instantiate Geometry.
     */
    Geometry();
};