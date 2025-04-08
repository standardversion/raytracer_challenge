#include <cmath>
#include "gtest/gtest.h"
#include "../colour.h"

/*
Scenario: Colours are (red, green, blue, alpha) tuples
  Given c ← colour(-0.5, 0.4, 1.7)
  Then c.red = -0.5
    And c.green = 0.4
    And c.blue = 1.7
    And c.alpha = 1.0
*/
TEST(colour, should_create_colour)
{
    const colour_t c{ -0.5, 0.4, 1.7 };
    EXPECT_EQ(c.red, -0.5);
    EXPECT_EQ(c.green, 0.4);
    EXPECT_EQ(c.blue, 1.7);
    EXPECT_EQ(c.alpha, 1.0);
}

/*
Scenario: Colour alpha values are clampped between 0 and 1
  Given c1 ← colour(-0.5, 0.4, 1.7, 1.1)
    And c2 ← colour(-0.5, 0.4, 1.7, -1.1)
  Then c1.alpha = 1.0
    And c2.alpha = 0.0
*/
TEST(colour, should_clamp_alpha_values_between_0_and_1)
{
    const colour_t c1{ -0.5, 0.4, 1.7, 1.1 };
    const colour_t c2{ -0.5, 0.4, 1.7, -1.1 };
    EXPECT_EQ(c1.alpha, 1.0);
    EXPECT_EQ(c2.alpha, 0.0);
}

/*
Scenario: Adding colours
  Given c1 ← colour(0.9, 0.6, 0.75)
    And c2 ← colour(0.7, 0.1, 0.25)
   Then c1 + c2 = colour(1.6, 0.7, 1.0)
*/
TEST(colour, should_be_able_to_add_two_colours_to_get_new_colour)
{
    const colour_t c1{ 0.9, 0.6, 0.75 };
    const colour_t c2{ 0.7, 0.1, 0.25 };
    const colour_t expected{ 1.6, 0.7, 1.0 };
    EXPECT_EQ(c1 + c2, expected);
}

/*
Scenario: Subtracting colours
  Given c1 ← colour(0.9, 0.6, 0.75)
    And c2 ← colour(0.7, 0.1, 0.25)
   Then c1 - c2 = colour(0.2, 0.5, 0.5)
*/
TEST(colour, should_be_able_to_subtract_two_colours_to_get_new_colour)
{
    const colour_t c1{ 0.9, 0.6, 0.75, 0.1 };
    const colour_t c2{ 0.7, 0.1, 0.25, 0.2 };
    const colour_t expected{ 0.2, 0.5, 0.5, 0.0 };
    EXPECT_EQ(c1 - c2, expected);
}

/*
Scenario: Multiplying a colour by a scalar
  Given c ← colour(0.2, 0.3, 0.4)
  Then c * 2 = colour(0.4, 0.6, 0.8)
*/
TEST(colour, should_be_able_to_multiply_colour_with_scalar)
{
    const colour_t c{ 0.2, 0.3, 0.4, 1.0 };
    const colour_t expected{ 0.4, 0.6, 0.8, 1.0 };
    EXPECT_EQ(c * 2, expected);
}

/*
Scenario: Multiplying colors
  Given c1 ← color(1, 0.2, 0.4)
    And c2 ← color(0.9, 1, 0.1)
   Then c1 * c2 = color(0.9, 0.2, 0.04)
*/
TEST(colour, should_be_able_to_multiply_two_colours_to_get_new_colour)
{
    const colour_t c1{ 1, 0.2, 0.4, 0.8 };
    const colour_t c2{ 0.9, 1, 0.1, 0.2 };
    const colour_t expected{ 0.9, 0.2, 0.04, 1.0 };
    EXPECT_EQ(c1 * c2, expected);
}

/*
Scenario: Convert to rgb 255 string
  Given c ← colour(1, 0.8, 0.6)
  Then c.to_rgb_255() = "255 204 153"
*/
TEST(colour, should_convert_to_rgb_255_string)
{
    const colour_t c{ 1, 0.8, 0.6 };
    EXPECT_STREQ(c.to_rgb_255().c_str(), "255 204 153");
}