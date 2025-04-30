#pragma once
#include <cmath>
#include "tuple.h"
#include "matrix.h"
#include "ray.h"

extern int bbox_tests;
/**
 * @brief Represents an axis-aligned bounding box in 3D space.
 *
 * Used for spatial queries like containment, bounding volume hierarchy,
 * and transforming geometry to world or parent space.
 */
struct bbox_t
{
    /* The minimum corner of the bounding box(smallest x, y, z values). */
    tuple_t min{ tuple_t::point(INFINITY, INFINITY, INFINITY) };

    /* The maximum corner of the bounding box (largest x, y, z values). */
    tuple_t max{ tuple_t::point(-INFINITY, -INFINITY, -INFINITY) };

    /**
     * @brief Expands the bounding box to include the given point(s).
     *
     * Can be called with one or more `tuple_t` points.
     *
     * @tparam Args Variadic template for additional points.
     * @param p First point to include.
     * @param args Additional points to include (optional).
     */
    template<typename ...Args>
    void add(const tuple_t& p, Args ...args);

    /**
     * @brief Checks if a point is inside the bounding box.
     *
     * @param p The point to test.
     * @return true if the point lies within or on the bounds.
     */
    bool contains(const tuple_t& p) const;

    /**
     * @brief Checks if another bounding box is fully contained within this box.
     *
     * @param box The bounding box to test.
     * @return true if the given box is entirely inside this bounding box.
     */
    bool contains(const bbox_t& box) const;

    /**
     * @brief Transforms the bounding box with the given matrix.
     *
     * The resulting bounding box is axis-aligned and contains the transformed
     * original box. This is useful when applying object-to-world or local-to-parent transforms.
     *
     * @param m The transformation matrix.
     * @return bbox_t The transformed bounding box.
     */
    bbox_t transform(const matrix_t& m) const;

    /**
     * @brief Tests if a ray intersects the bounding box.
     *
     * Performs an axis-aligned bounding box (AABB) intersection test
     * using the given ray.
     *
     * @param r The ray to test for intersection.
     * @return true if the ray intersects the bounding box, false otherwise.
     */
    bool intersect(const ray_t& r) const;

    std::pair<bbox_t, bbox_t> split() const;

    /**
     * @brief Expands this bounding box to include another bounding box.
     *
     * @param bb The bounding box to merge with this one.
     * @return bbox_t& Reference to this bounding box.
     */
    bbox_t& operator+=(const bbox_t& bb);
};


template<typename... Args>
void bbox_t::add(const tuple_t& p, Args... args)
{
	if (p.x < min.x) min.x = p.x;
	if (p.y < min.y) min.y = p.y;
	if (p.z < min.z) min.z = p.z;
	if (p.x > max.x) max.x = p.x;
	if (p.y > max.y) max.y = p.y;
	if (p.z > max.z) max.z = p.z;
	if constexpr (sizeof...(args) > 0)
	{
		add(args...);
	}
}