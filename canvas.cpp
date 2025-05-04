#include <algorithm>
#include "canvas.h"
#include "ppm.h"

canvas_t::canvas_t(const int w, const int h)
	: width{ w }, height{ h }
{
	colour_t black{ 0, 0, 0 };
	colour_buffer = new colour_t[width * height];
	std::fill(colour_buffer, colour_buffer + (width * height), black);
}

canvas_t::canvas_t(const ppm_t& ppm)
	: width{ ppm.width }, height{ ppm.height }
{
	colour_buffer = new colour_t[width * height];
	/*for (const auto& colour : ppm.colour_data)
	{
		std::fill(colour_buffer, colour_buffer + (width * height), colour);
	}*/
	for (int y{ 0 }; y < height; y++)
	{
		for (int x{ 0 }; x < width; x++)
		{
			const int index{ (width * y) + x };
			write_pixel(x, y, ppm.colour_data[index]);
		}
	}
}

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