#pragma once
#include <memory>
#include "geometry.h"
#include "triangle.h"
#include "wavefront_obj.h"
#include "bvh.h"

/**
 * @class Mesh
 * @brief Represents a 3D mesh composed of triangles, typically loaded from a Wavefront OBJ file.
 *
 * A mesh is a collection of flat or smooth-shaded triangles that can be intersected with rays.
 * It can be constructed from either a parsed `wavefront_t` object or directly from a file path.
 * Smooth shading can be toggled, allowing for either flat or interpolated vertex normals.
 */
class Mesh : public Geometry
{
public:
    /** @brief A collection of triangle primitives making up the mesh geometry. */
    std::vector<std::shared_ptr<Triangle>> triangles;

    std::unique_ptr<bvh_t> bvh;

    /** @brief Whether the mesh should use smooth shading (per-vertex normals). */
    bool smooth{ false };
    
    /** @brief Bounding box of the mesh. */
    bbox_t bbox{};
    /**
     * @brief Default constructor for an empty mesh.
     *
     * Creates an empty mesh with no geometry or data loaded.
     * Typically used when manually constructing a mesh by adding triangles later.
     */
    Mesh();

    /**
     * @brief Constructs a mesh from a parsed wavefront OBJ structure.
     * @param obj The parsed Wavefront OBJ data.
     * @param smooth Whether to use smooth shading (default is true).
     */
    Mesh(const wavefront_t& obj, bool smooth = true);

    /**
     * @brief Constructs a mesh by loading and parsing an OBJ file.
     * @param obj_filename The path to the OBJ file.
     * @param smooth Whether to use smooth shading (default is true).
     */
    Mesh(const char* obj_filename, bool smooth = true);

    /**
     * @brief Factory method to create a shared pointer to a Mesh from parsed OBJ data.
     * @param obj The parsed Wavefront OBJ data.
     * @param smooth Whether to use smooth shading (default is true).
     * @param bvh_threshold The threshold (number of tris) for creating bounding volume hierarchy
     * @return Shared pointer to the newly created Mesh.
     */
    static std::shared_ptr<Mesh> create(const wavefront_t& obj, bool smooth = true, int bvh_threshold = 128);

    /**
     * @brief Factory method to create a shared pointer to a Mesh from a file.
     * @param obj_filename The path to the OBJ file.
     * @param smooth Whether to use smooth shading (default is true).
     * @param bvh_threshold The threshold (number of tris) for creating bounding volume hierarchy
     * @return Shared pointer to the newly created Mesh.
     */
    static std::shared_ptr<Mesh> create(const char* obj_filename, bool smooth = true, int bvh_threshold = 128);

    /**
     * @brief Computes the intersection(s) between a ray and all triangles in the mesh.
     * @param local_ray The ray in the mesh's local space.
     * @param intersections A container to collect all resulting intersections.
     */
    void local_intersect(const ray_t& local_ray, intersections_t& intersections) const override;

    /**
     * @brief Computes the normal at a given point on the mesh using barycentric coordinates.
     * @param local_point The point on the mesh in local space (may be unused depending on shading mode).
     * @param alpha Barycentric coordinate corresponding to the first triangle vertex.
     * @param beta Barycentric coordinate corresponding to the second triangle vertex.
     * @param gamma Barycentric coordinate corresponding to the third triangle vertex.
     * @return The surface normal at the specified point.
     */
    tuple_t local_normal_at(const tuple_t& local_point, const double alpha = 0, const double beta = 0, const double gamma = 0) const override;

    /**
     * @brief Returns the bounding box of the object.
     *
     * Overrides the base class implementation to provide the specific bounding box
     * for this derived class.
     *
     * @return bbox_t The bounding box of the object.
     */
    bbox_t bounds() const override;

    /**
     * @brief Builds a BVH tree for the mesh using the specified triangle threshold.
     *
     * This function creates a root BVH node, adds all triangles from the mesh,
     * and recursively builds the hierarchy by splitting nodes whose triangle count
     * exceeds the given threshold.
     *
     * @param threshold Maximum number of triangles allowed per BVH node before it is split.
     */
    void create_bvh(int threshold);

};