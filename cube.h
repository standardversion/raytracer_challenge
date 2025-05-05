#pragma once
#include <utility>
#include "ray.h"
#include "matrix.h"
#include "tuple.h"
#include "material.h"
#include "geometry.h"
#include "uv.h"

struct intersections_t;

/**
 * @enum Cube_Faces
 * @brief Identifies the faces of a cube.
 *
 * Used for determining which face a point lies on.
 */
enum Cube_Faces
{
    left,   ///< Left face (negative X)
    right,  ///< Right face (positive X)
    front,  ///< Front face (negative Z)
    back,   ///< Back face (positive Z)
    up,     ///< Top face (positive Y)
    down    ///< Bottom face (negative Y)
};

/**
 * @class Cube
 * @brief Represents an axis-aligned unit cube centered at the origin.
 *
 * Inherits from Geometry and provides implementations for ray intersection,
 * surface normal calculation, and UV mapping specific to cubes.
 */
class Cube : public Geometry
{
public:

    /**
     * @brief Factory method to create a new Cube instance.
     *
     * @return A std::shared_ptr pointing to a new Cube object.
     */
    static std::shared_ptr<Cube> create();

    /**
     * @brief Computes the UV coordinates for a point on the cube's surface.
     *
     * Maps a point on a cube face to 2D UV texture coordinates in [0, 1] range.
     *
     * @param point A point on the cube in local space.
     * @return uv_t The corresponding UV coordinates.
     */
    uv_t get_uv(const tuple_t& point) const override;

    /**
     * @brief Identifies which face of the cube a point lies on.
     *
     * Used internally to determine how to map points to UV coordinates or normals.
     *
     * @param p A point on or near the surface of the cube.
     * @return Cube_Faces Enum value indicating the face the point belongs to.
     */
    static Cube_Faces face_from_point(const tuple_t& p);

    /**
     * @brief Intersects a ray with the cube in local object space.
     *
     * Computes the intersections between a ray and the cube, if any,
     * and appends valid hits to the given intersection collection.
     *
     * @param local_ray The ray transformed into the cube's local coordinate space.
     * @param intersections A reference to the collection where intersections will be added.
     */
    void local_intersect(const ray_t& local_ray, intersections_t& intersections) const override;

    /**
     * @brief Computes the surface normal vector at a given point on the cube.
     *
     * Assumes the point is in object-local space and returns a vector
     * perpendicular to the corresponding cube face.
     *
     * @param local_point The point on the cube’s surface.
     * @param alpha Barycentric alpha value (unused for cubes).
     * @param beta Barycentric beta value (unused for cubes).
     * @param gamma Barycentric gamma value (unused for cubes).
     * @return A unit vector (tuple_t) representing the surface normal at the point.
     */
    tuple_t local_normal_at(const tuple_t& local_point, const double alpha = 0, const double beta = 0, const double gamma = 0) const override;

    /**
     * @brief Returns the bounding box of the cube.
     *
     * The bounding box spans from (-1, -1, -1) to (1, 1, 1), matching the cube's geometry.
     *
     * @return bbox_t The bounding box enclosing the cube.
     */
    bbox_t bounds() const override;

    /**
     * @brief Computes UV coordinates for a point on a cube face (static version).
     *
     * A utility method that can be used independently of any instance to map a point
     * on the cube surface to UV space.
     *
     * @param point The point on the cube in local space.
     * @return uv_t The UV coordinates corresponding to the face the point lies on.
     */
    static uv_t static_get_uv(const tuple_t& point);

private:
    /**
     * @brief Helper function to compute the intersection range along a single axis.
     *
     * Used internally to compute tmin and tmax values for a ray against the cube's axis-aligned faces.
     *
     * @param origin The ray origin's coordinate on the specific axis.
     * @param direction The ray direction along that axis.
     * @return A pair of (tmin, tmax) values for this axis.
     */
    std::pair<double, double> check_axis(const double origin, const double direction) const;
};