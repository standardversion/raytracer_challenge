#pragma once
#include <vector>
#include "canvas.h"
#include "camera.h"
#include "world.h"
#include "thread_pool.h"

/**
 * @struct render_tile_t
 * @brief Represents a rectangular tile region for rendering.
 *
 * Each tile defines a subregion of the full image with start and end bounds
 * in both the x (horizontal) and y (vertical) directions. These tiles are used
 * to divide the rendering task into smaller parallel units.
 */
struct render_tile_t
{
    int x_start; ///< Starting x-coordinate (inclusive)
    int x_end;   ///< Ending x-coordinate (exclusive)
    int y_start; ///< Starting y-coordinate (inclusive)
    int y_end;   ///< Ending y-coordinate (exclusive)
};

/**
 * @class RenderManager
 * @brief Manages tile-based rendering of a scene using a thread pool.
 *
 * The RenderManager splits the camera's output image into rectangular tiles and uses
 * a thread pool to render each tile in parallel. It handles task distribution and timing.
 */
class RenderManager
{
public:
    /**
     * @brief Constructs the RenderManager with a given camera and tile size.
     *
     * @param camera The camera defining the view and resolution.
     * @param tile_size The width and height of each tile (in pixels).
     */
    RenderManager(const Camera& camera, const int tile_size);

    /**
     * @brief Renders the entire scene described by the world.
     *
     * This method divides the image into tiles and dispatches each tile
     * as a task to the internal thread pool. Returns the completed image.
     *
     * @param world The world (scene) to be rendered.
     * @return canvas_t The rendered image.
     */
    canvas_t render(const World& world);

private:
    Camera render_camera;                       ///< The camera used for rendering.
    int tile_size;                              ///< The size of each render tile.
    std::vector<render_tile_t> render_tiles;    ///< The list of all tiles to render.
    ThreadPool thread_pool{};                   ///< Internal thread pool used for rendering.
};

