#pragma once
#include "scene_object.h"
#include "matrix.h"
#include "ray.h"
#include "canvas.h"
#include "world.h"

/**
 * @class Camera
 * @brief Represents a pinhole camera used to render a scene from a specific viewpoint.
 *
 * The camera defines the view into the 3D world, including its resolution, field of view,
 * transformation (position/orientation), and projection math. It is capable of generating
 * rays for individual pixels and rendering an entire scene to a canvas.
 */
class Camera : public SceneObject
{
public:
	/**
	 * @brief Horizontal size (width) of the rendered image in pixels.
	 */
	int hsize;

	/**
	 * @brief Vertical size (height) of the rendered image in pixels.
	 */
	int vsize;

	/**
	 * @brief Field of view in radians, describing the extent of the observable scene.
	 */
	double field_of_view;

	/**
	 * @brief Half the width of the view at a distance of 1 from the camera.
	 */
	double half_width;

	/**
	 * @brief Half the height of the view at a distance of 1 from the camera.
	 */
	double half_height;

	/**
	 * @brief Size of a single pixel in world units.
	 */
	double pixel_size;

	/**
	 * @brief Transformation matrix for positioning and orienting the camera in world space.
	 */
	matrix_t transform{ matrix_t::identity() };

	/**
	 * @brief Constructs a camera with the given image dimensions and field of view.
	 * @param hsize The horizontal resolution in pixels.
	 * @param vsize The vertical resolution in pixels.
	 * @param fov The field of view in radians.
	 */
	Camera(const int hsize, const int vsize, const double fov);

	/**
	 * @brief Computes the ray that starts at the camera and passes through the given pixel.
	 * @param x The horizontal pixel coordinate.
	 * @param y The vertical pixel coordinate.
	 * @return A ray representing the line of sight from the camera through the pixel.
	 */
	ray_t ray_for_pixel(const int x, const int y) const;

	/**
	 * @brief Renders the given world from the perspective of this camera.
	 * @param world The 3D world to render.
	 * @return A canvas containing the rendered image.
	 */
	canvas_t render(const World& world) const;
};

