#include "mesh_group.h"
#include "mesh.h"

void mesh_grp_t::build(int threshold) {
    if (triangles.size() <= threshold) return;
    bbox = {};
    for (const auto& tri : triangles) {
        bbox += tri->bounds();
    }
    auto [left_box, right_box] = bbox.split();
    auto left_grp = std::make_shared<mesh_grp_t>();
    auto right_grp = std::make_shared<mesh_grp_t>();

    bool moved_any = false;
    /*for (const auto& tri : triangles) {
        const tuple_t centroid{ tri->bounds().centroid() };
        if (left_box.contains(centroid)) {
            left_grp->add(tri);
            moved_any = true;
        }
        else if (right_box.contains(centroid)) {
            right_grp->add(tri);
            moved_any = true;
        }
    }*/

    for (const auto& tri : triangles) {
        const bbox_t tb{ tri->bounds() };
        if (left_box.contains(tb)) {
            left_grp->add(tri);
            moved_any = true;
        }
        else if (right_box.contains(tb)) {
            right_grp->add(tri);
            moved_any = true;
        }
    }

    if (moved_any) {
        if (left_grp->triangles.size() == triangles.size() || right_grp->triangles.size() == triangles.size()) {
            return; // no meaningful split
        }
        if (!left_grp->triangles.empty()) {
            left_grp->build(threshold);
            mesh_grps.push_back(left_grp);
        }
        if (!right_grp->triangles.empty()) {
            right_grp->build(threshold);
            mesh_grps.push_back(right_grp);
        }

        // Remove moved triangles
        triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
            [&](const auto& t) {
                auto box = t->bounds();
                return left_box.contains(box) || right_box.contains(box);
            }), triangles.end());
    }
}

void mesh_grp_t::add(std::shared_ptr<Triangle> t) {
	triangles.push_back(t);
	bbox += t->bounds();
}

void mesh_grp_t::local_intersect(const ray_t& local_ray, intersections_t& intersections) const {
    if (!bbox.intersect(local_ray)) return;

    for (const auto& tri : triangles) {
        tri->local_intersect(local_ray, intersections);
    }

    for (const auto& child : mesh_grps) {
        child->local_intersect(local_ray, intersections);
    }
}

