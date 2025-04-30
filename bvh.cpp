#include "bvh.h"
#include "mesh.h"
#include <iostream>

void bvh_t::build(int threshold) {
    if (triangles.size() <= threshold) return;
    bbox = {};
    for (const auto& tri : triangles)
    {
        bbox += tri->bounds();
    }
    auto [left_box, right_box] = bbox.split();
    auto left_bvh = std::make_shared<bvh_t>();
    auto right_bvh = std::make_shared<bvh_t>();

    bool moved_any = false;
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

    if (moved_any) {
        if (left_bvh->triangles.size() == triangles.size() || right_bvh->triangles.size() == triangles.size())
        {
            return; // no meaningful split
        }

        if (!left_bvh->triangles.empty())
        {
            left_bvh->build(threshold);
            bvhs.push_back(left_bvh);
        }
        if (!right_bvh->triangles.empty())
        {
            right_bvh->build(threshold);
            bvhs.push_back(right_bvh);
        }

        // Remove moved triangles
        triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
            [&](const auto& t) {
                auto box = t->bounds();
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

