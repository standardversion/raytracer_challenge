#pragma once
#include <memory>
#include "ray.h"
#include "intersection.h"
#include "bounding_box.h"
#include "triangle.h"

/**
 * @brief Bounding Volume Hierarchy (BVH) node for accelerating ray-triangle intersection tests.
 *
 * A BVH partitions space hierarchically using axis-aligned bounding boxes (AABBs),
 * allowing efficient pruning during ray intersection queries. Each BVH node can
 * hold a collection of triangles and/or child BVH nodes.
 */
struct bvh_t
{
    /**
     * @brief Child BVH nodes created during recursive build.
     *
     * These are the result of spatial partitioning of triangles
     * when a node exceeds the triangle threshold.
     */
    std::vector<std::shared_ptr<bvh_t>> bvhs;

    /**
     * @brief Triangles stored in this node.
     *
     * If the node is a leaf or splitting was not possible,
     * triangles remain here.
     */
    std::vector<std::shared_ptr<Triangle>> triangles;

    /**
     * @brief Bounding box enclosing all triangles in this node.
     *
     * Updated during triangle insertion and BVH construction.
     */
    bbox_t bbox;

    /**
     * @brief Builds the BVH by recursively partitioning triangles based on their bounding boxes.
     *
     * If the number of triangles in this node exceeds the given threshold,
     * attempts to split the bounding box and distribute triangles to child nodes.
     * Recursively continues building for each child if they also exceed the threshold.
     *
     * @param threshold The maximum number of triangles a node may contain before splitting.
     */
    void build(int threshold);

    /**
     * @brief Adds a triangle to this BVH node.
     *
     * The triangle is appended to the `triangles` list,
     * and the bounding box is expanded to include its bounds.
     *
     * @param t Shared pointer to the triangle to add.
     */
    void add(std::shared_ptr<Triangle> t);

    /**
     * @brief Performs intersection tests between a ray and the BVH hierarchy.
     *
     * If the ray intersects this node's bounding box, it tests all triangles
     * in this node, then recursively tests all child nodes.
     * All intersections found are appended to the `intersections` collection.
     *
     * @param local_ray The ray in local space.
     * @param intersections The container to collect any intersection hits.
     */
    void local_intersect(const ray_t& local_ray, intersections_t& intersections) const;
};
