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
     * @param p1 First vertex of the triangle.
     * @param p2 Second vertex of the triangle.
     * @param p3 Third vertex of the triangle.
     */
    Triangle(const tuple_t& p1, const tuple_t& p2, const tuple_t& p3);

    /** @brief The first vertex of the triangle. */
    tuple_t p1;

    /** @brief The second vertex of the triangle. */
    tuple_t p2;

    /** @brief The third vertex of the triangle. */
    tuple_t p3;

    /** @brief Edge vector from p1 to p2 (e1 = p2 - p1). */
    tuple_t e1;

    /** @brief Edge vector from p1 to p3 (e2 = p3 - p1). */
    tuple_t e2;

    /** @brief The surface normal of the triangle (normalized cross product of e1 and e2). */
    tuple_t normal;

    /**
     * @brief Factory method to create a shared pointer to a Triangle.
     * @param p1 First vertex.
     * @param p2 Second vertex.
     * @param p3 Third vertex.
     * @return Shared pointer to the newly created Triangle.
     */
    static std::shared_ptr<Triangle> create(const tuple_t& p1, const tuple_t& p2, const tuple_t& p3);

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
    tuple_t local_normal_at(const tuple_t& local_point) const override;
};


