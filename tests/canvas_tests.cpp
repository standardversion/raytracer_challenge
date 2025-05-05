#include <cmath>
#include "gtest/gtest.h"
#include "../canvas.h"
#include "../colour.h"
#include "../ppm.h"

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

/*
Scenario: Reading a PPM returns a canvas of the right size
  Given ppm ← a file containing:
	"""
	P3
	5 3
	255
	255 0 0 255 0 0 255 0 0 255 0 0 255 0 0
	255 0 0 255 0 0 255 0 0 255 0 0 255 0 0
	255 0 0 255 0 0 255 0 0 255 0 0 255 0 0
	"""
  When canvas ← canvas_from_ppm(ppm)
  Then canvas.width = 5
	And canvas.height = 3
*/
TEST(canvas, should_set_width_height_as_per_ppm)
{
	const ppm_t ppm{ "..\\..\\tests\\in.ppm" };
	canvas_t c{ ppm };
	EXPECT_EQ(c.width, 5);
	EXPECT_EQ(c.height, 3);
}

/*
Scenario: Reading pixel data from a PPM file
  Given ppm ← a file containing:
	"""
	P3
	4 3
	255
	255 127 0  0 127 255  127 255 0  255 255 255
	0 0 0  255 0 0  0 255 0  0 0 255
	255 255 0  0 255 255  255 0 255  127 127 127
	"""
  When canvas ← canvas_from_ppm(ppm)
  Then pixel_at(canvas, <x>, <y>) = <color>

  Examples:
	| x | y | color                      |
	| 0 | 0 | color(1, 0.498, 0)         |
	| 1 | 0 | color(0, 0.498, 1)         |
	| 2 | 0 | color(0.498, 1, 0)         |
	| 3 | 0 | color(1, 1, 1)             |
	| 0 | 1 | color(0, 0, 0)             |
	| 1 | 1 | color(1, 0, 0)             |
	| 2 | 1 | color(0, 1, 0)             |
	| 3 | 1 | color(0, 0, 1)             |
	| 0 | 2 | color(1, 1, 0)             |
	| 1 | 2 | color(0, 1, 1)             |
	| 2 | 2 | color(1, 0, 1)             |
	| 3 | 2 | color(0.498, 0.498, 0.498) |
*/
TEST(canvas, should_read_pixel_data_from_ppm)
{
	const ppm_t ppm{ "..\\..\\tests\\in2.ppm" };
	canvas_t c{ ppm };
	EXPECT_EQ(c.pixel_at(0, 0), colour_t(1, 0.498, 0));
	EXPECT_EQ(c.pixel_at(1, 0), colour_t(0, 0.498, 1));
	EXPECT_EQ(c.pixel_at(2, 0), colour_t(0.498, 1, 0));
	EXPECT_EQ(c.pixel_at(3, 0), colour_t(1, 1, 1));
	EXPECT_EQ(c.pixel_at(0, 1), colour_t(0, 0, 0));
	EXPECT_EQ(c.pixel_at(1, 1), colour_t(1, 0, 0));
	EXPECT_EQ(c.pixel_at(2, 1), colour_t(0, 1, 0));
	EXPECT_EQ(c.pixel_at(3, 1), colour_t(0, 0, 1));
	EXPECT_EQ(c.pixel_at(0, 2), colour_t(1, 1, 0));
	EXPECT_EQ(c.pixel_at(1, 2), colour_t(0, 1, 1));
	EXPECT_EQ(c.pixel_at(2, 2), colour_t(1, 0, 1));
	EXPECT_EQ(c.pixel_at(3, 2), colour_t(0.498, 0.498, 0.498));
}

/*
Scenario: PPM parsing ignores comment lines
  Given ppm ← a file containing:
	"""
	P3
	# this is a comment
	2 1
	# this, too
	255
	# another comment
	255 255 255
	# oh, no, comments in the pixel data!
	255 0 255
	"""
  When canvas ← canvas_from_ppm(ppm)
  Then pixel_at(canvas, 0, 0) = color(1, 1, 1)
	And pixel_at(canvas, 1, 0) = color(1, 0, 1)
*/
TEST(canvas, should_ignore_comments)
{
	const ppm_t ppm{ "..\\..\\tests\\in3.ppm" };
	canvas_t c{ ppm };
	EXPECT_EQ(c.pixel_at(0, 0), colour_t(1, 1, 1));
	EXPECT_EQ(c.pixel_at(1, 0), colour_t(1, 0, 1));
}

/*
Scenario: PPM parsing allows an RGB triple to span lines
  Given ppm ← a file containing:
	"""
	P3
	1 1
	255
	51
	153

	204
	"""
  When canvas ← canvas_from_ppm(ppm)
  Then pixel_at(canvas, 0, 0) = color(0.2, 0.6, 0.8)
*/
TEST(canvas, should_handle_multi_line_colours)
{
	const ppm_t ppm{ "..\\..\\tests\\in4.ppm" };
	canvas_t c{ ppm };
	EXPECT_EQ(c.pixel_at(0, 0), colour_t(0.2, 0.6, 0.8));
}

/*
Scenario: PPM parsing respects the scale setting
  Given ppm ← a file containing:
	"""
	P3
	2 2
	100
	100 100 100  50 50 50
	75 50 25  0 0 0
	"""
  When canvas ← canvas_from_ppm(ppm)
  Then pixel_at(canvas, 0, 1) = color(0.75, 0.5, 0.25)
*/
TEST(canvas, should_respect_scale_setting)
{
	const ppm_t ppm{ "..\\..\\tests\\in5.ppm" };
	canvas_t c{ ppm };
	EXPECT_EQ(c.pixel_at(0, 1), colour_t(0.75, 0.5, 0.25));
}