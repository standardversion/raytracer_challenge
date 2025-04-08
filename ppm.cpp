#include "ppm.h"
#include "utils.h"
#include <vector>

// CONSTRUCTOR
ppm_t::ppm_t(const canvas_t& canvas, int max_chars)
{
	data += "P3\n";
	data += std::to_string(canvas.width) + " " + std::to_string(canvas.height) + "\n";
	data += "255\n";

	for (int y{ 0 }; y < canvas.height; y++)
	{
		std::string line{};
		for (int x{ 0 }; x < canvas.width; x++)
		{
			const colour_t colour{ canvas.pixel_at(x, y) };
			std::string rgb_255{ colour.to_rgb_255() };
			line += rgb_255;
			if (x != canvas.width - 1)
			{
				line += " ";
			}
		}
		if (line.size() <= max_chars)
		{
			data += line + "\n";
		}
		else
		{
			std::vector<std::string> splitz{ split(line, " ") };
			std::string line_fixed_size{};
			for (const auto& value : splitz)
			{
				if (line_fixed_size.size() <= (max_chars - 5)) //colour value will be max 3 chars + line termination \n 2 chars so - 5
				{
					line_fixed_size += value + " ";
				}
				else
				{
					line_fixed_size.pop_back(); // remove whitespace
					data += line_fixed_size + "\n";
					line_fixed_size.clear();
					line_fixed_size += value + " ";
				}
			}
			if (line_fixed_size.size())
			{
				line_fixed_size.pop_back(); // remove whitespace
				data += line_fixed_size + "\n";
			}
			
		}
	}
	data += "\n";
}
