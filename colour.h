#pragma once
#include <string>

struct colour_t
{
	// MEMBER VARIABLES
	double red;
	double green;
	double blue;
	double alpha{ 1.0 };

	// CONSTRUCTORS
	/**
	 * @brief Default constructor for the `colour_t` class.
	 *
	 * This constructor initializes a `colour_t` object with default values. The color
	 * components (red, green, blue) are initialized to zero, resulting in a color of
	 * black (RGB: 0.0, 0.0, 0.0).
	 *
	 * This constructor is automatically generated by the compiler when the default
	 * constructor is not explicitly defined, and it initializes the data members with
	 * their default values.
	 */
	colour_t() = default;

	/**
	 * @brief Constructs a colour_t object with specified red, green, blue, and optional alpha values.
	 *
	 * This constructor initializes the colour using the provided red, green, and blue values,
	 * and optionally an alpha value for transparency. If the alpha value is not provided, it defaults
	 * to 1.0, which represents full opacity.
	 *
	 * @param r The red component of the colour (0.0 to 1.0).
	 * @param g The green component of the colour (0.0 to 1.0).
	 * @param b The blue component of the colour (0.0 to 1.0).
	 * @param a The alpha (opacity) component of the colour, defaulting to 1.0 (full opacity).
	 */
	colour_t(double r, double g, double b, double a = 1.0);

	// MEMBER FUNCTIONS
	std::string to_rgb_255() const;

	// OPERATORS
	/**
	 * @brief Compares two colours for equality.
	 *
	 * This function checks whether two colours are equal by comparing their individual components.
	 * Two colours are considered equal if all corresponding components are equal.
	 *
	 * @param c The colour to compare against.
	 * @return `true` if the colour are equal, `false` otherwise.
	 */
	bool operator==(const colour_t& c) const;

	/**
	 * @brief Adds two colours component-wise.
	 *
	 * This function performs component-wise addition of two colours. The result is a new colour
	 * where each component is the sum of the corresponding components from the two input colours.
	 * Alpha is clamped between 0.0 and 1.0
	 *
	 * @param t The colour to add.
	 * @return A new colour representing the result of the addition.
	 */
	colour_t operator+(const colour_t& c) const;

	/**
	 * @brief Subtracts one colour from another component-wise.
	 *
	 * This function performs component-wise subtraction of two colours. The result is a new colour
	 * where each component is the difference of the corresponding components from the two input colours.
	 * Alpha is clamped between 0.0 and 1.0
	 *
	 * @param t The colour to subtract.
	 * @return A new colour representing the result of the subtraction.
	 */
	colour_t operator-(const colour_t& c) const;

	/**
	 * @brief Multiplies the colour by a scalar.
	 *
	 * This function multiplies each component of the colour by a scalar factor and returns a new colour.
	 * Alpha is clamped between 0.0 and 1.0
	 *
	 * @param factor The scalar factor to multiply each component by.
	 * @return A new colour representing the scaled colour.
	 */
	colour_t operator*(const double factor) const;

	/**
	 * @brief Multiplies the colour by another colour.
	 *
	 * This function performs component-wise multiplication of two colours. The result is a new colour
	 * where each component is the multiple of the corresponding components from the two input colours.
	 * Alpha is clamped between 0.0 and 1.0
	 *
	 * @param t The colour to multiply.
	 * @return A new colour representing the multiplied colour.
	 */
	colour_t operator*(const colour_t& c) const;
};
