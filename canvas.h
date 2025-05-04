#pragma once
#include "colour.h"

struct ppm_t;
/**
 * @brief Represents a 2D drawing canvas.
 *
 * A canvas is a rectangular grid of pixels, where each pixel holds a colour value.
 * It is typically used for rendering final images in a raytracer.
 */
struct canvas_t
{
	/**
	 * @brief The width of the canvas in pixels.
	 */
	int width;

	/**
	 * @brief The height of the canvas in pixels.
	 */
	int height;

	/**
	 * @brief Pointer to the colour buffer storing pixel data.
	 *
	 * This is a flat array of `colour_t` values with size `width * height`,
	 * typically laid out in row-major order.
	 */
	colour_t* colour_buffer{ nullptr };

	/**
	 * @brief Constructs a canvas of the given width and height, and initializes
	 *        all pixels to black.
	 *
	 * This constructor dynamically allocates memory for the canvas' pixel data
	 * and initializes all pixels to the color black (RGB: 0, 0, 0). The canvas size
	 * is determined by the provided width and height.
	 *
	 * @param w The width of the canvas.
	 * @param h The height of the canvas.
	 */
	canvas_t(const int w, const int h);

	canvas_t(const ppm_t& ppm);

	/**
	 * @brief Writes a color value to a specific pixel on the canvas.
	 *
	 * This method sets the color of the pixel at the specified `(x, y)` coordinates
	 * on the canvas to the given color. The coordinates are zero-based, with `(0, 0)`
	 * being the top-left corner.
	 *
	 * @param x The x-coordinate (horizontal position) of the pixel to modify.
	 * @param y The y-coordinate (vertical position) of the pixel to modify.
	 * @param colour The color to assign to the pixel.
	 *
	 * @note This function assumes that the coordinates `(x, y)` are within the bounds
	 *       of the canvas.
	 */
	void write_pixel(const int x, const int y, const colour_t& colour);

	/**
	 * @brief Fills the entire canvas with a specified color.
	 *
	 * This function assigns the specified color to every pixel on the canvas. It uses
	 * `std::fill` to overwrite all existing pixel values in the color buffer with the
	 * given color. The canvas is completely filled, regardless of its previous state.
	 *
	 * @param colour The color to fill the entire canvas with.
	 */
	void fill(const colour_t& colour);

	/**
	 * @brief Retrieves the color of a specific pixel on the canvas.
	 *
	 * This method returns the color of the pixel located at the specified `(x, y)`
	 * coordinates on the canvas. The coordinates are zero-based, with `(0, 0)` being
	 * the top-left corner. If the coordinates are out of bounds, the behavior is undefined.
	 *
	 * @param x The x-coordinate (horizontal position) of the pixel to retrieve.
	 * @param y The y-coordinate (vertical position) of the pixel to retrieve.
	 *
	 * @return The color of the pixel at the specified coordinates.
	 *
	 * @note This function assumes that the coordinates `(x, y)` are within the bounds
	 *       of the canvas. It does not perform bounds checking, so invalid coordinates
	 *       may result in undefined behavior.
	 */
	colour_t pixel_at(const int x, const int y) const;
};
