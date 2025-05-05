#pragma once
#include "vector"
#include "pattern.h"

/**
 * @class CubeMap
 * @brief A pattern that maps different textures or colors to each face of a cube.
 *
 * The CubeMap class allows for applying a unique pattern to each face of a cube.
 * It determines which face a point lies on and delegates the color calculation
 * to the corresponding pattern.
 */
class CubeMap : public Pattern
{
public:
    /**
     * @brief Constructs a CubeMap with a pattern for each cube face.
     *
     * Each face of the cube is assigned a specific pattern. The order corresponds to:
     * left (-X), right (+X), front (-Z), back (+Z), up (+Y), and down (-Y).
     *
     * @param left Pattern for the left face (-X).
     * @param right Pattern for the right face (+X).
     * @param front Pattern for the front face (-Z).
     * @param back Pattern for the back face (+Z).
     * @param up Pattern for the top face (+Y).
     * @param down Pattern for the bottom face (-Y).
     */
    CubeMap(
        std::shared_ptr<Pattern> left,
        std::shared_ptr<Pattern> right,
        std::shared_ptr<Pattern> front,
        std::shared_ptr<Pattern> back,
        std::shared_ptr<Pattern> up,
        std::shared_ptr<Pattern> down
    );

    /**
     * @brief Returns the color at a given point on the cube.
     *
     * Determines which face of the cube the point lies on and uses the
     * corresponding face pattern to compute the final color.
     *
     * @param point A point on the cube in object-local space.
     * @return The color at the specified point.
     */
    colour_t at(const tuple_t& point) const override;

private:
    /**
     * @brief A vector of patterns for each face of the cube.
     *
     * The order is: left, right, front, back, up, down.
     */
    std::vector<std::shared_ptr<Pattern>> patterns;
};