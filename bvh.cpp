#include "bvh.h"
#include "mesh.h"
#include <iostream>

void bvh_t::build(int threshold) {
    // If the number of triangles is less than or equal to the threshold, we stop and don't build further
    if (triangles.size() <= threshold) return;

    // Initialize an empty bounding box to accumulate the bounds of all triangles
    bbox = {};

    // Combine the bounds of all the triangles into a single bounding box
    for (const auto& tri : triangles)
    {
        bbox += tri->bounds();
    }

    // Split the bounding box into two parts (left and right)
    auto [left_box, right_box] = bbox.split();

    // Create two child BVH nodes (left and right)
    auto left_bvh = std::make_shared<bvh_t>();
    auto right_bvh = std::make_shared<bvh_t>();

    // Flag to check if any triangles were moved to the left or right child
    bool moved_any = false;

    // Distribute the triangles between the left and right child BVH nodes based on their bounds
    for (const auto& tri : triangles)
    {
        const bbox_t tb{ tri->bounds() };
        if (left_box.contains(tb))
        {
            left_bvh->add(tri);
            moved_any = true;
        }
        else if (right_box.contains(tb))
        {
            right_bvh->add(tri);
            moved_any = true;
        }
    }

    // If any triangles were moved, proceed with building child BVH nodes
    if (moved_any) {
        // Check if all triangles are in one of the child BVHs, which means no meaningful split
        if (left_bvh->triangles.size() == triangles.size() || right_bvh->triangles.size() == triangles.size())
        {
            return; 
        }

        // Recursively build the left and right BVHs if they contain any triangles
        if (!left_bvh->triangles.empty())
        {
            left_bvh->build(threshold);
            bvhs.push_back(left_bvh);    // Add the left BVH node to the parent BVH's list
        }
        if (!right_bvh->triangles.empty())
        {
            right_bvh->build(threshold);
            bvhs.push_back(right_bvh);    // Add the right BVH node to the parent BVH's list
        }

        // Remove the triangles that have been moved to the left or right child BVHs
        triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
            [&](const auto& t) {
                auto box = t->bounds();
                // Remove triangles that are now contained in either the left or right bounding box
                return left_box.contains(box) || right_box.contains(box);
            }), triangles.end());
    }
}

void bvh_t::add(std::shared_ptr<Triangle> t) {
	triangles.push_back(t);
	bbox += t->bounds();
}

void bvh_t::local_intersect(const ray_t& local_ray, intersections_t& intersections) const {
    if (!bbox.intersect(local_ray)) return;

    for (const auto& tri : triangles) {
        tri->local_intersect(local_ray, intersections);
    }

    for (const auto& child : bvhs) {
        child->local_intersect(local_ray, intersections);
    }
}

