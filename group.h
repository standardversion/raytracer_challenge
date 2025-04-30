#pragma once
#include <vector>
#include <memory>
#include "scene_object.h"
#include "bounding_box.h"

struct intersections_t;

/**
 * @brief Represents a group of scene objects that share a common transformation.
 *
 * A Group can contain any number of SceneObjects (including other Groups),
 * and applies its transformation to all of its children. Useful for hierarchical
 * scene structures like models made of multiple parts.
 */
class Group : public SceneObject
{
public:    
    /**
     * @brief The child objects contained within this group.
     *
     * Each child may be a renderable shape or another group.
     */
    std::vector<std::shared_ptr<SceneObject>> children{};

    /**
     * @brief Adds one or more SceneObjects to the group.
     *
     * Each object will have its parent pointer set to this group.
     * This function is variadic and can accept multiple objects in a single call.
     *
     * @tparam Args Variadic list of additional SceneObject pointers.
     * @param obj The first object to add.
     * @param args Additional objects to add (optional).
     */
    template<typename... Args>
    void add(std::shared_ptr<SceneObject> obj, Args... args);

    /**
     * @brief Finds a specific object within the group.
     *
     * Performs a shallow search through the group's immediate children.
     *
     * @param obj The object to find.
     * @return A pointer to the object if found, otherwise nullptr.
     */
    std::shared_ptr<SceneObject> find(const SceneObject* obj) const;

    /**
     * @brief Computes intersections between a ray and all renderable children.
     *
     * The ray is transformed into the group's local space before testing children.
     * Each child's intersections (if any) are added to the provided list.
     *
     * @param ray The ray to test for intersection.
     * @param intersections The list to store any intersections found.
     */
    void intersect(const ray_t& ray, intersections_t& intersections) const;

    /**
     * @brief Returns the bounding box of the group excluding non renderables.
     *
     * Overrides the base class implementation to provide the specific bounding box
     * for this derived class.
     *
     * @return bbox_t The bounding box of the group.
     */
    bbox_t bounds() const;
};


template<typename... Args>
void Group::add(std::shared_ptr<SceneObject> obj, Args... args)
{
	if (obj->parent.lock().get() != this)
	{
		children.push_back(obj);
		obj->parent = shared_from_this();
	}
	if constexpr (sizeof...(args) > 0) {  // Check if there are more arguments
		add(args...);  // Recursively call add for the rest of the arguments
	}
}