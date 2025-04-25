#include <cmath>
#include <algorithm>
#include "colour.h"
#include "settings.h"

colour_t::colour_t(double r, double g, double b, double a)
	: red{ r }, green{ g }, blue{ b }
{
	if (a > 1.0)
	{
		alpha = 1.0;
	}
	if (a < 0.0)
	{
		alpha = 0.0;
	}
}

std::string colour_t::to_rgb_255() const
{
	int r_255{ (int)std::round(red * 255) };
	r_255 = (r_255 > 255) ? 255 : (r_255 < 0) ? 0 : r_255;
	int g_255{ (int)std::round(green * 255) };
	g_255 = (g_255 > 255) ? 255 : (g_255 < 0) ? 0 : g_255;
	int b_255{ (int)std::round(blue * 255) };
	b_255 = (b_255 > 255) ? 255 : (b_255 < 0) ? 0 : b_255;
	return std::to_string(r_255) + " " + std::to_string(g_255) + " " + std::to_string(b_255);
}

bool colour_t::operator==(const colour_t& c) const
{
	const double red_comp{ std::fabs(this->red - c.red) };
	const double green_comp{ std::fabs(this->green - c.green) };
	const double blue_comp{ std::fabs(this->blue - c.blue) };
	const double alpha_comp{ std::fabs(this->alpha - c.alpha) };
	if (red_comp < EPSILON && green_comp < EPSILON && blue_comp < EPSILON && alpha_comp < EPSILON)
	{
		return true;
	}
	return false;
}

colour_t colour_t::operator+(const colour_t& c) const
{
	return {
		this->red + c.red,
		this->green + c.green,
		this->blue + c.blue,
		this->alpha + c.alpha > 1.0 ? 1.0 : this->alpha + c.alpha
	};
}

colour_t& colour_t::operator+=(const colour_t& c)
{
	this->red += c.red;
	this->green += c.green;
	this->blue += c.blue;
	this->alpha = std::min(this->alpha + c.alpha, 1.0);
	return *this;
}

colour_t colour_t::operator-(const colour_t& c) const
{
	return {
		this->red - c.red,
		this->green - c.green,
		this->blue - c.blue,
		this->alpha - c.alpha < 0.0 ? 0.0 : this->alpha - c.alpha
	};
}

colour_t colour_t::operator*(const double factor) const
{
	return {
		this->red * factor,
		this->green * factor,
		this->blue * factor,
		this->alpha * factor > 1.0 ? 1.0 : this->alpha * factor
	};
}

colour_t colour_t::operator*(const colour_t& c) const
{
	return {
		this->red * c.red,
		this->green * c.green,
		this->blue * c.blue,
		this->alpha * c.alpha > 1.0 ? 1.0 : this->alpha * c.alpha
	};
}

colour_t colour_t::operator/(const double factor) const
{
	return {
		this->red / factor,
		this->green / factor,
		this->blue / factor,
		this->alpha / factor > 1.0 ? 1.0 : this->alpha / factor
	};
}