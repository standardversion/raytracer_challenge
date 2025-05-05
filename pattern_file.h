#pragma once
#include "canvas.h"
#include "pattern.h"

/**
 * @class PatternFile
 * @brief A pattern that maps image data from a file onto geometry surfaces using UV coordinates.
 *
 * PatternFile loads an image from disk and samples its pixels based on UV coordinates provided during rendering.
 * It supports full UV-based color lookups and is useful for texture mapping. The image data is stored internally
 * in a canvas, which handles resolution, pixel access, and interpolation if needed.
 */
class PatternFile : public Pattern
{
public:
    /**
     * @brief The file path of the loaded image.
     *
     * Stores the original file path used to load the image for reference or debugging.
     */
    const char* file;

    /**
     * @brief The canvas holding the image pixel data.
     *
     * Internally stores the loaded image and provides color sampling functionality.
     */
    canvas_t canvas;

    /**
     * @brief Constructs a PatternFile by loading an image from disk.
     *
     * Loads the image at the specified file path into the internal canvas. The image
     * must be in a supported format (e.g., PPM, PNG, etc. depending on implementation).
     *
     * @param filepath Path to the image file to load as a texture pattern.
     */
    PatternFile(const char* filepath);

    /**
     * @brief Indicates that this pattern supports UV coordinate mapping.
     *
     * Overrides the base implementation to return true.
     *
     * @return true, indicating this pattern uses UV mapping.
     */
    bool supports_uv() const override;

    /**
     * @brief Computes the color at a 3D point in pattern space.
     *
     * This method is required by the base Pattern class but is not typically used
     * for image-based patterns. It may return a default color or delegate to UV lookup.
     *
     * @param point A point in pattern space.
     * @return colour_t The resulting color at the point.
     */
    colour_t at(const tuple_t& point) const override;

    /**
     * @brief Computes the color at the given UV coordinates from the loaded image.
     *
     * Samples the canvas using the UV coordinates, typically in [0, 1] range, to
     * retrieve the corresponding pixel color.
     *
     * @param u Horizontal UV coordinate.
     * @param v Vertical UV coordinate.
     * @return colour_t The color from the image at the specified UV.
     */
    colour_t at(const double u, const double v) const override;
};

