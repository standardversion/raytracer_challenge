#pragma once
#pragma once
#include "ray.h"
#include "matrix.h"
#include "tuple.h"
#include "material.h"
#include "geometry.h"

struct intersections_t;

/**
 * @class Triangle
 * @brief Represents a triangle geometry used in ray tracing.
 *
 * A flat-shaded triangle defined by three points (vertices). It stores precomputed edge vectors
 * and a surface normal for efficient intersection and shading calculations.
 */
class Triangle : public Geometry
{
public:

    /**
     * @brief Constructs a triangle from three points.
     * @param v1 First vertex of the triangle.
     * @param p2 Second vertex of the triangle.
     * @param v3 Third vertex of the triangle.
     */
    Triangle(const tuple_t& v1, const tuple_t& v2, const tuple_t& v3);

    Triangle
    (
        const tuple_t& v1,
        const tuple_t& v2,
        const tuple_t& v3,
        const std::pair<double, double>& v1_uv,
        const std::pair<double, double>& v2_uv,
        const std::pair<double, double>& v3_uv
    );

    /** @brief The first vertex of the triangle. */
    tuple_t v1;

    /** @brief The second vertex of the triangle. */
    tuple_t v2;

    /** @brief The third vertex of the triangle. */
    tuple_t v3;

    /** @brief Edge vector from v1 to p2 (e1 = p2 - v1). */
    tuple_t e1;

    /** @brief Edge vector from v1 to v3 (e2 = v3 - v1). */
    tuple_t e2;

    /** @brief The surface normal of the triangle (normalized cross product of e1 and e2). */
    tuple_t normal;

    std::pair<double, double> v1_uv;
    std::pair<double, double> v2_uv;
    std::pair<double, double> v3_uv;

    /**
     * @brief Factory method to create a shared pointer to a Triangle.
     * @param v1 First vertex.
     * @param p2 Second vertex.
     * @param v3 Third vertex.
     * @return Shared pointer to the newly created Triangle.
     */
    static std::shared_ptr<Triangle> create(const tuple_t& v1, const tuple_t& v2, const tuple_t& v3);

    /**
     * @brief Computes the intersection(s) between a ray and the triangle in local space.
     * @param local_ray The ray in the object's local space.
     * @param intersections A container to store the resulting intersection(s), if any.
     */
    void local_intersect(const ray_t& local_ray, intersections_t& intersections) const override;

    /**
     * @brief Computes the surface normal at a given point on the triangle (constant across surface).
     * @param local_point The point on the triangle in local space (not used for flat shading).
     * @return The normal vector at the given point.
     */
    tuple_t local_normal_at(const tuple_t& local_point, const double alpha = 0, const double beta = 0, const double gamma = 0) const override;
};


