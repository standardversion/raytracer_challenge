#pragma once
#include "matrix.h"
#include "ray.h"

/**
 * @brief Base class for all objects in the scene that can be transformed.
 *
 * Provides common properties such as a unique ID, a transformation matrix,
 * and a renderable flag. Intended to be inherited by objects like geometry,
 * lights, and cameras.
 */
class SceneObject
{
public:
    /**
     * @brief A unique identifier for the object.
     *
     * Automatically assigned when the object is constructed.
     */
    int id{ 0 };

    /**
     * @brief The transformation matrix applied to the object.
     *
     * Used to position, scale, or rotate the object in world space.
     * Defaults to the identity matrix.
     */
    matrix_t transform{ matrix_t::identity() };

    /**
     * @brief Whether the object should be considered in rendering/intersection.
     *
     * Useful for non-geometry objects (like lights or cameras) that exist in the
     * scene but don't participate in ray intersections.
     */
    bool renderable{ false };

    /**
 * @brief The parent of this scene object in the scene graph.
 *
 * Used to apply hierarchical transformations. If nullptr, this object
 * is at the root of the scene graph.
 */
    SceneObject* parent{ nullptr };

    /**
     * @brief Converts a point from world space into the object’s local space.
     *
     * Recursively applies the inverse transformations of all parent objects
     * to transform the point from world space into the local coordinate system
     * of this object.
     *
     * @param point The point in world space.
     * @return The equivalent point in this object’s local space.
     */
    tuple_t world_to_object(const tuple_t& point) const;

    /**
     * @brief Converts a normal vector from object space into world space.
     *
     * Applies the transpose of the inverse of the combined world transform,
     * accounting for non-uniform scaling and other effects. Recursively includes
     * parent transformations.
     *
     * @param normal The normal vector in object space.
     * @return The equivalent normal vector in world space.
     */
    tuple_t normal_to_world(const tuple_t& normal) const;

    /**
     * @brief Virtual destructor for SceneObject.
     *
     * Required to safely delete derived objects via base class pointers,
     * and to enable runtime type checking (e.g. via dynamic_cast).
     */
    virtual ~SceneObject() = default; // make polymorphic so can use dynamic_cast

protected:
    /**
     * @brief Protected constructor to enforce inheritance.
     *
     * Automatically sets a unique ID for the scene object.
     */
    SceneObject();

    /**
     * @brief Global counter for assigning unique IDs to each SceneObject.
     */
    static inline int sceneobj_id_counter{ 1 };
};
