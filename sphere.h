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
 * normal calculation and intersection detection. Also supports shared ownership
 * through std::enable_shared_from_this to safely return shared pointers to itself.
 */
class Sphere : public Geometry, public std::enable_shared_from_this<Sphere>
{
public:
	/**
	 * @brief The radius of the sphere.
	 *
	 * Defaults to 1.0, representing a unit sphere centered at the origin.
	 */
	double radius{ 1.0 };

	/**
	 * @brief Creates a new shared pointer to a Sphere instance.
	 *
	 * Ensures proper usage of `shared_from_this()` within the class. All spheres
	 * should be created through this method to avoid ownership issues.
	 *
	 * @param radius The radius of the sphere (default is 1.0).
	 * @return std::shared_ptr<Sphere> A shared pointer to the created Sphere.
	 */
	static std::shared_ptr<Sphere> create(double radius = 1.0);

	/**
	 * @brief Computes intersections between the sphere and a given ray.
	 *
	 * @param ray The ray to test for intersection with this sphere.
	 * @param intersections A reference to an intersections_t collection where results will be stored.
	 *
	 * Determines the points (if any) at which the ray intersects the sphere, and
	 * adds those intersection points (typically as time values and object pointers)
	 * to the provided `intersections` collection. Handles rays originating inside
	 * or outside the sphere, and accounts for all valid real intersections.
	 */
	void intersect(const ray_t& ray, intersections_t& intersections) const override;

	/**
	* @brief Calculates the normal vector at a given point on the sphere in local space.
	*
	* For a perfect sphere centered at the origin, the normal at a point is simply the vector
	* from the origin to that point, normalized.
	*
	* @param local_point A point on the sphere in object-local coordinates.
	* @return tuple_t The normal vector at the point.
	*/
	tuple_t local_normal_at(const tuple_t& local_point) const override;
private:

	/**
	 * @brief Constructs a sphere with a specified radius.
	 *
	 * @param r The radius of the sphere. Defaults to 1.0 if not specified.
	 *
	 * Initializes a sphere centered at the origin with radius `r`. The sphere may
	 * later be transformed or associated with material properties, depending on
	 * your implementation.
	 */
	Sphere(const double r = 1.0);

};
