#pragma once
#include <memory>
#include "geometry.h"
#include "triangle.h"
#include "wavefront_obj.h"

class Mesh : public Geometry
{
public:
    std::vector<std::shared_ptr<Triangle>> triangles;

    bool smooth{ false };

    Mesh(const wavefront_t& obj, bool smooth = true);

    Mesh(const char* obj_filename, bool smooth = true);

    static std::shared_ptr<Mesh> create(const wavefront_t& obj, bool smooth = true);

    static std::shared_ptr<Mesh> create(const char* obj_filename, bool smooth = true);

    void local_intersect(const ray_t& local_ray, intersections_t& intersections) const override;

    tuple_t local_normal_at(const tuple_t& local_point, const double alpha = 0, const double beta = 0, const double gamma = 0) const override;
};

