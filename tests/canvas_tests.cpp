#include <cmath>
#include "gtest/gtest.h"
#include "../canvas.h"
#include "../colour.h"

/*
Scenario: Creating a canvas
  Given c ← canvas(10, 20)
  Then c.width = 10
    And c.height = 20
    And every pixel of c is color(0, 0, 0)
*/
TEST(canvas, should_create_canvas_with_given_width_height_and_each_pixel_should_be_black_colour)
{
    const canvas_t c{ 10, 20 };
    EXPECT_EQ(c.width, 10);
    EXPECT_EQ(c.height, 20);
	const colour_t black{ 0, 0, 0 };
	for (int y{ 0 }; y < c.height; y++)
	{
		for (int x{ 0 }; x < c.width; x++)
		{
			EXPECT_EQ(c.colour_buffer[(c.width * y) + x], black);
		}
	}
}

/*
Scenario: Writing pixels to a canvas
  Given c ← canvas(10, 20)
	And red ← color(1, 0, 0)
  When c.write_pixel(2, 3, red)
  Then c.pixel_at(2, 3) = red
*/
TEST(canvas, should_be_able_write_read_colur_at_x_y_pixel)
{
	canvas_t c{ 10, 20 };
	EXPECT_EQ(c.width, 10);
	const colour_t red{ 1, 0, 0 };
	c.write_pixel(2, 3, red);
	EXPECT_EQ(c.pixel_at(2, 3), red);
}

/*
Scenario: Filling canvas with a colour
  Given c ← canvas(5, 5)
	And red ← color(1, 0, 0)
  When c.fill(red)
  Then c.pixel_at(every x, every y) = red
*/
TEST(canvas, should_fill_colour_buffer_with_colour)
{
	canvas_t c{ 5, 5 };
	const colour_t red{ 1, 0, 0 };
	c.fill(red);
	for (int y{ 0 }; y < c.height; y++)
	{
		for (int x{ 0 }; x < c.width; x++)
		{
			EXPECT_EQ(c.colour_buffer[(c.width * y) + x], red);
		}
	}
}