#pragma once
#include "geometry.h"
#include "wavefront_obj.h"

class Mesh : public Geometry
{
public:
    /** @brief List of vertex positions. */
    std::vector<tuple_t> vertices;

    /**
     * @brief List of per-vertex normals (unaveraged).
     * Each entry is a list of normals associated with the same vertex position.
     */
    std::vector<std::vector<tuple_t>> vertex_normals;

    /**
     * @brief Averaged normal vectors per vertex.
     * Computed from `vertex_normals` to support smooth shading.
     */
    std::vector<tuple_t> vertex_normals_avg;

    /** @brief List of 2D texture coordinates (UVs). */
    std::vector<std::pair<double, double>> uvs;

    /** @brief List of triangle faces defined in the .obj file. */
    std::vector<face_t> faces;

    bool smooth{ false };

    Mesh(const wavefront_t& obj);

    Mesh(const char* obj_filename);

    static std::shared_ptr<Mesh> create(const wavefront_t& obj);

    static std::shared_ptr<Mesh> create(const char* obj_filename);

    void local_intersect(const ray_t& local_ray, intersections_t& intersections) const override;

    tuple_t local_normal_at(const tuple_t& local_point, const double alpha = 0, const double beta = 0, const double gamma = 0) const override;
};

