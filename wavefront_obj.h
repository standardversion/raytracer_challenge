#pragma once
#include <vector>
#include <optional>
#include "tuple.h"

/**
 * @struct face_t
 * @brief Represents a single face (triangle) in a Wavefront .obj file.
 *
 * Stores vertex indices, UV indices, and normal indices for each corner of the triangle.
 * All indices are expected to be 0-based (adjusted from .obj's 1-based indexing during parsing).
 */
struct face_t
{
    /** @brief Index of the first vertex (position). */
    int a;

    /** @brief Index of the second vertex (position). */
    int b;

    /** @brief Index of the third vertex (position). */
    int c;

    /** @brief Index of the first vertex's UV coordinate. */
    std::optional<int> a_uv;

    /** @brief Index of the second vertex's UV coordinate. */
    std::optional<int> b_uv;

    /** @brief Index of the third vertex's UV coordinate. */
    std::optional<int> c_uv;

    /** @brief Index of the first vertex's normal vector. */
    std::optional<int> a_normal;

    /** @brief Index of the second vertex's normal vector. */
    std::optional<int> b_normal;

    /** @brief Index of the third vertex's normal vector. */
    std::optional<int> c_normal;

    bool has_uvs() const;

    bool has_normals() const;
};

/**
 * @struct wavefront_t
 * @brief Represents the full data parsed from a Wavefront .obj file.
 *
 * Stores all geometry and attribute data including vertices, normals, UVs, and faces.
 */
struct wavefront_t
{
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

    /**
     * @brief Constructs and loads a Wavefront object file.
     * @param obj_filename Path to the .obj file to load.
     */
    wavefront_t(const char* obj_filename);
};
