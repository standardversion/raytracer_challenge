#pragma once
#include "triangle.h"

/**
 * @class SmoothTriangle
 * @brief Represents a triangle with per-vertex normals for smooth shading.
 *
 * In contrast to a flat-shaded triangle, this triangle uses vertex normals (n1, n2, n3)
 * and barycentric interpolation to compute the surface normal at any point on the triangle.
 * Useful for realistic lighting and shading effects.
 */
class SmoothTriangle : public Triangle
{
public:
    /** @brief The vertex normal of the triangle at vertex v1. */
    tuple_t n1;

    /** @brief The vertex normal of the triangle at vertex v2. */
    tuple_t n2;

    /** @brief The vertex normal of the triangle at vertex v3. */
    tuple_t n3;

    /**
     * @brief Constructs a smooth triangle with UVs and per-vertex normals.
     * @param v1 First vertex of the triangle.
     * @param v2 Second vertex of the triangle.
     * @param v3 Third vertex of the triangle.
     * @param v1_uv Texture coordinates at the first vertex.
     * @param v2_uv Texture coordinates at the second vertex.
     * @param v3_uv Texture coordinates at the third vertex.
     * @param n1 Normal at the first vertex.
     * @param n2 Normal at the second vertex.
     * @param n3 Normal at the third vertex.
     */
    SmoothTriangle
    (
        const tuple_t& v1,
        const tuple_t& v2,
        const tuple_t& v3,
        const std::pair<double, double>& v1_uv,
        const std::pair<double, double>& v2_uv,
        const std::pair<double, double>& v3_uv,
        const tuple_t& n1,
        const tuple_t& n2,
        const tuple_t& n3
    );

    /**
     * @brief Factory method to create a shared pointer to a SmoothTriangle.
     * @param v1 First vertex.
     * @param v2 Second vertex.
     * @param v3 Third vertex.
     * @param v1_uv Texture coordinates at the first vertex.
     * @param v2_uv Texture coordinates at the second vertex.
     * @param v3_uv Texture coordinates at the third vertex.
     * @param n1 Normal at the first vertex.
     * @param n2 Normal at the second vertex.
     * @param n3 Normal at the third vertex.
     * @return Shared pointer to the newly created SmoothTriangle.
     */
    static std::shared_ptr<SmoothTriangle> create(
        const tuple_t& v1,
        const tuple_t& v2,
        const tuple_t& v3,
        const std::pair<double, double>& v1_uv,
        const std::pair<double, double>& v2_uv,
        const std::pair<double, double>& v3_uv,
        const tuple_t& n1,
        const tuple_t& n2,
        const tuple_t& n3
    );

    /**
     * @brief Computes the interpolated normal at a point on the triangle using barycentric coordinates.
     * @param local_point The point on the triangle in local space (unused in this implementation).
     * @param alpha Barycentric coordinate corresponding to vertex v1.
     * @param beta Barycentric coordinate corresponding to vertex v2.
     * @param gamma Barycentric coordinate corresponding to vertex v3.
     * @return The interpolated normal vector at the given point.
     */
    tuple_t local_normal_at(const tuple_t& local_point, const double alpha = 0, const double beta = 0, const double gamma = 0) const override;
};

