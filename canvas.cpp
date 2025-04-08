#include <algorithm>
#include "canvas.h"

// CONSTRUCTOR
canvas_t::canvas_t(const int w, const int h)
	: width{ w }, height{ h }
{
	colour_t black{ 0, 0, 0 };
	colour_buffer = new colour_t[width * height];
	std::fill(colour_buffer, colour_buffer + (width * height), black);
}

// MEMBER FUNCTIONS
void canvas_t::write_pixel(const int x, const int y, const colour_t& colour)
{
	const int index{ (width * y) + x };
	if (index >= 0 && index < (width * height))
	{
		colour_buffer[index] = colour;
	}
}

void canvas_t::fill(const colour_t& colour)
{
	std::fill(colour_buffer, colour_buffer + (width * height), colour);
}

colour_t canvas_t::pixel_at(const int x, const int y) const
{
	const int index{ (width * y) + x };
	if (index >= 0 && index < (width * height))
	{
		return colour_buffer[index];
	}
}