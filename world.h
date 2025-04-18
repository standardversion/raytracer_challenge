#pragma once
#include <vector>
#include <memory>
#include "colour.h"
#include "scene_object.h"
#include "geometry.h"
#include "light.h"
#include "ray.h"
#include "intersection.h"
#include "intersection_state.h"

/**
 * @class World
 * @brief Represents a 3D scene containing objects, lights, and the logic to render them.
 *
 * The World class manages all scene objects and lighting elements. It provides
 * methods for ray-object intersection, shading, and determining visible colors
 * based on lighting and material interactions.
 */
class World
{
public:
	/**
	 * @brief A list of all scene objects in the world.
	 *
	 * These are owned by the world and may include lights, geometry, and other components.
	 */
	std::vector<std::unique_ptr<SceneObject>> scene_objects;

	/**
	 * @brief A list of geometry objects that can be rendered.
	 *
	 * These are raw pointers to Geometry objects extracted from the scene_objects list.
	 */
	std::vector<Geometry*> renderables;

	/**
	 * @brief A list of light sources in the world.
	 *
	 * These affect how objects in the scene are shaded.
	 */
	std::vector<Light*> lights;

	/**
	 * @brief Creates a default world with predefined objects and lighting.
	 *
	 * @return A World instance containing a standard setup, typically used for testing
	 * or as a base scene.
	 */
	static World default_world();

	/**
	 * @brief Adds a new object to the world.
	 *
	 * @param obj A unique pointer to a SceneObject to add (geometry, light, etc.).
	 *
	 * The object is stored and, if renderable, added to the appropriate internal list.
	 */
	void add_object(std::unique_ptr<SceneObject> obj);

	/**
	 * @brief Intersects a ray with all renderable objects in the world.
	 *
	 * @param ray The ray to test against the scene.
	 * @param intersections An output collection to store intersection results.
	 *
	 * All intersected objects contribute to the resulting intersection list,
	 * which is typically sorted afterward to find the closest hit.
	 */
	void intersect(const ray_t& ray, intersections_t& intersections) const;

	/**
	 * @brief Computes the color at the point of intersection.
	 *
	 * @param state The precomputed intersection state, including point, normal, etc.
	 * @return The final shaded color at the intersection point.
	 *
	 * This method performs lighting calculations (e.g., diffuse, specular)
	 * using the intersection data and the world's lights.
	 */
	colour_t shade_hit(const intersection_state& state, int remaining) const;

	colour_t reflected_colour(const intersection_state& state, int remaining) const;

	/**
	 * @brief Computes the color seen along a given ray.
	 *
	 * @param ray The ray being cast into the scene.
	 * @return The resulting color at the first visible intersection, or background color if no hit occurs.
	 */
	colour_t colour_at(const ray_t& ray, int remaining) const;

	/**
	 * @brief Determines whether a point in the world is in shadow relative to a light source.
	 *
	 * @param point The point in world space to test for shadowing.
	 * @param light The specific light source to test visibility against.
	 * @return true if the point is in shadow (i.e., occluded from the light source), false otherwise.
	 *
	 * This function casts a ray from the point toward each light in the scene
	 * and checks for any objects obstructing the path. Used during shading to apply shadow effects.
	 */
	bool is_shadowed(const tuple_t point, Light* light) const;

};


