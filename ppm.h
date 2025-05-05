#pragma once
#include <string>
#include <vector>
#include "canvas.h"
#include "colour.h"

/**
 * @struct ppm_t
 * @brief Represents an image encoded in the PPM (Portable Pixmap) format.
 *
 * The PPM format is a simple, human-readable image format often used
 * in ray tracing for output or debugging. This struct stores a PPM-formatted
 * string representation of an image and supports conversion from a canvas
 * or loading from a file.
 */
struct ppm_t
{
    /**
     * @brief The full PPM image data as a string.
     *
     * Includes the header and pixel values formatted per the PPM standard.
     * Can be written directly to a `.ppm` file.
     */
    std::string data{};

    /**
     * @brief Width of the image in pixels.
     */
    int width;

    /**
     * @brief Height of the image in pixels.
     */
    int height;

    /**
     * @brief Maximum color channel value (typically 255).
     */
    int max_colour_value = 255;

    /**
     * @brief Raw color data of the image, stored in row-major order.
     */
    std::vector<colour_t> colour_data;

    /**
     * @brief Constructs a PPM representation from a canvas.
     *
     * Converts the pixel data in the canvas into PPM format, optionally
     * enforcing a maximum line length for readability.
     *
     * @param canvas The canvas to convert into a PPM image.
     * @param max_chars Maximum characters per line in the PPM output (default is 70).
     *
     * @note The resulting `data` string can be written directly to a `.ppm` file.
     */
    ppm_t(const canvas_t& canvas, int max_chars = 70);

    /**
     * @brief Constructs a PPM image by reading from a PPM file on disk.
     *
     * Parses the contents of a valid PPM file and initializes the object with
     * the parsed data and metadata.
     *
     * @param filepath Path to the PPM file to read.
     * @throws std::runtime_error If the file is missing, unreadable, or invalid.
     */
    ppm_t(const char* filepath);

    /**
     * @brief Writes the PPM data to a file.
     *
     * Outputs the current contents of the `data` string to the specified file path.
     * The file will be overwritten if it already exists.
     *
     * @param filepath The target file path for the output.
     * @throws std::runtime_error If the file cannot be opened or written.
     */
    void write_to_file(const char* filepath) const;
};