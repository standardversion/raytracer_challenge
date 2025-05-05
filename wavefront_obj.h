#pragma once
#include <vector>
#include <optional>
#include "tuple.h"

/**
 * @struct face_t
 * @brief Represents a single triangular face in a Wavefront `.obj` file.
 *
 * Each face consists of three vertex indices and optionally includes
 * UV texture coordinate indices and normal vector indices. All indices
 * are expected to be zero-based (i.e., adjusted from `.obj` format's 1-based indexing).
 */
struct face_t
{
    /** @brief Index of the first vertex position. */
    int a;

    /** @brief Index of the second vertex position. */
    int b;

    /** @brief Index of the third vertex position. */
    int c;

    /** @brief Index of the first vertex's UV coordinate (optional). */
    std::optional<int> a_uv;

    /** @brief Index of the second vertex's UV coordinate (optional). */
    std::optional<int> b_uv;

    /** @brief Index of the third vertex's UV coordinate (optional). */
    std::optional<int> c_uv;

    /** @brief Index of the first vertex's normal vector (optional). */
    std::optional<int> a_normal;

    /** @brief Index of the second vertex's normal vector (optional). */
    std::optional<int> b_normal;

    /** @brief Index of the third vertex's normal vector (optional). */
    std::optional<int> c_normal;

    /**
     * @brief Checks whether the face includes UV texture coordinates.
     * @return true if all three UV indices are set; false otherwise.
     */
    bool has_uvs() const;

    /**
     * @brief Checks whether the face includes normal vector indices.
     * @return true if all three normal indices are set; false otherwise.
     */
    bool has_normals() const;
};

/**
 * @struct wavefront_t
 * @brief Represents geometry and attribute data parsed from a Wavefront `.obj` file.
 *
 * This struct stores all the information extracted from a `.obj` file, including
 * vertex positions, normals, UVs, and triangular face definitions.
 * It also handles normal averaging for smooth shading if normals are provided.
 */
struct wavefront_t
{
    /** @brief List of vertex positions (3D points). */
    std::vector<tuple_t> vertices;

    /**
     * @brief Per-vertex normals as raw input (unaveraged).
     * Each vertex may be associated with multiple normals, one for each face.
     */
    std::vector<std::vector<tuple_t>> vertex_normals;

    /**
     * @brief Averaged normal per vertex.
     *
     * Computed from `vertex_normals`, this is used for smooth shading and
     * replaces flat shading per face when available.
     */
    std::vector<tuple_t> vertex_normals_avg;

    /** @brief List of 2D UV coordinates for texture mapping. */
    std::vector<std::pair<double, double>> uvs;

    /** @brief List of triangular faces composed of vertex/UV/normal indices. */
    std::vector<face_t> faces;

    /**
     * @brief Constructs and loads a Wavefront `.obj` file into memory.
     *
     * Parses the specified file and populates the vertex, normal, UV, and face data.
     *
     * @param obj_filename Path to the `.obj` file to be loaded.
     * @throws std::runtime_error If the file is not found or contains invalid syntax.
     */
    wavefront_t(const char* obj_filename);
};

