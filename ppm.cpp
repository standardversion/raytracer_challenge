#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <vector>
#include "ppm.h"
#include "utils.h"

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

ppm_t::ppm_t(const char* filepath)
{
	std::ifstream infile(filepath);
	if (!infile) {
		throw std::runtime_error("PPM file not found");
	}

	std::stringstream buffer;
	buffer << infile.rdbuf();
	data = buffer.str(); // keep full string

	std::istringstream parser(data);
	std::vector<std::string> tokens = get_clean_tokens(parser, '#');

	if (tokens.size() < 4) {
		throw std::invalid_argument("PPM file too short or missing metadata");
	}

	// Parse metadata
	const std::string& magic = tokens[0];
	if (magic != "P3") {
		throw std::invalid_argument("PPM file must start with P3");
	}

	width = std::stoi(tokens[1]);
	height = std::stoi(tokens[2]);
	max_colour_value = std::stoi(tokens[3]);

	if (width <= 0 || height <= 0 || max_colour_value <= 0) {
		throw std::invalid_argument("Invalid width/height/max_color_value");
	}

	// Parse pixel data
	std::size_t i = 4;
	while (i + 2 < tokens.size()) {
		int r = std::stoi(tokens[i++]);
		int g = std::stoi(tokens[i++]);
		int b = std::stoi(tokens[i++]);

		colour_data.emplace_back(
			static_cast<double>(r) / max_colour_value,
			static_cast<double>(g) / max_colour_value,
			static_cast<double>(b) / max_colour_value
		);
	}

	if (colour_data.size() != width * height) {
		throw std::runtime_error("PPM pixel count doesn't match width � height");
	}
}


void ppm_t::write_to_file(const char* filepath) const
{
	std::ofstream outfile(filepath);
	if (outfile.is_open())
	{
		outfile << data;
		outfile.close();
	}
	else
	{
		std::runtime_error("Error while writing PPM file");
	}
}