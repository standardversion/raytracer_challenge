#pragma once
#include "ray.h"
#include "matrix.h"
#include "tuple.h"
#include "material.h"
#include "geometry.h"

struct intersections_t;

/**
 * @brief Represents a 3D sphere shape that can be intersected by rays.
 *
 * Inherits from Geometry and implements sphere-specific logic such as
 * normal calculation and intersection detection.
 * Designed for exclusive ownership via std::shared_ptr.
 */
class Sphere : public Geometry
{
public:

    explicit Sphere(double radius = 1.0);
    /**
     * @brief The radius of the sphere.
     *
     * Defaults to 1.0, representing a unit sphere centered at the origin.
     */
    double radius{ 1.0 };

    /**
     * @brief Creates a new unique pointer to a Sphere instance.
     *
     * All spheres should be created through this method to ensure consistent construction
     * and ownership. Unique ownership ensures efficient memory management.
     *
     * @param radius The radius of the sphere (default is 1.0).
     * @return std::shared_ptr<Sphere> A unique pointer to the created Sphere.
     */
    static std::shared_ptr<Sphere> create(double radius = 1.0);

    /**
     * @brief Creates a new unique pointer to a transparent glass Sphere instance.
     *
     * Constructs a sphere with default properties for glass: fully transparent,
     * with a refractive index of 1.5. Use this factory method when simulating
     * realistic glass or other transparent materials.
     *
     * @param radius The radius of the sphere (default is 1.0).
     * @return std::shared_ptr<Sphere> A unique pointer to the created glass Sphere.
     */
    static std::shared_ptr<Sphere> glass_sphere(double radius = 1.0);

    /**
     * @brief Computes intersections between the sphere and a given ray in object space.
     *
     * @param ray The ray to test for intersection with this sphere.
     * @param intersections A reference to an intersections_t collection where results will be stored.
     */
    void local_intersect(const ray_t& local_ray, intersections_t& intersections) const override;

    /**
     * @brief Calculates the normal vector at a given point on the sphere in local space.
     *
     * @param local_point A point on the sphere in object-local coordinates.
     * @return tuple_t The normal vector at the point.
     */
    tuple_t local_normal_at(const tuple_t& local_point) const override;
};

