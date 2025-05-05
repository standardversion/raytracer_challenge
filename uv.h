#pragma once

/**
 * @struct uv_t
 * @brief Represents a 2D texture coordinate (UV) used in texture mapping.
 *
 * A UV coordinate pair (`u`, `v`) defines a point on a 2D texture map. The `u` and `v`
 * values are typically normalized between 0 and 1, where (0,0) is the bottom-left corner
 * and (1,1) is the top-right corner of the texture.
 */
struct uv_t {
    /** @brief The U-coordinate of the texture (horizontal axis). */
    double u;

    /** @brief The V-coordinate of the texture (vertical axis). */
    double v;

    /**
     * @brief Compares two `uv_t` objects for equality.
     *
     * This operator returns `true` if both the `u` and `v` components are equal
     * between the two `uv_t` objects.
     *
     * @param c The `uv_t` object to compare against.
     * @return true if the `u` and `v` values are the same; false otherwise.
     */
    bool operator==(const uv_t& c) const;
};

