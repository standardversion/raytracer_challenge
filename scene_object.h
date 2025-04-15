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

