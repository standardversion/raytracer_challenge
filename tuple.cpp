#include <cmath>
#include <stdexcept>
#include "tuple.h"

tuple_t tuple_t::point(const double x, const double y, const double z)
{
	return { x, y, z, 1.0 };
}

tuple_t tuple_t::vector(const double x, const double y, const double z)
{
	return { x, y, z, 0.0 };
}

bool tuple_t::is_point() const
{
	return w == 1.0 ? true : false;
}

bool tuple_t::is_vector() const
{
	return w == 1.0 ? false : true;
}

bool tuple_t::operator==(const tuple_t& t) const
{
	const double x_comp{ std::fabs(this->x - t.x) };
	const double y_comp{ std::fabs(this->y - t.y) };
	const double z_comp{ std::fabs(this->z - t.z) };
	const double w_comp{ std::fabs(this->w - t.w) };
	if (x_comp < EPSILON && y_comp < EPSILON && z_comp < EPSILON && w_comp < EPSILON)
	{
		return true;
	}
	return false;
}

tuple_t tuple_t::operator+(const tuple_t& t) const
{
	if (this->is_point() && t.is_point())
	{
		throw std::invalid_argument("Cannot add two points");
	}
	return {
		this->x + t.x,
		this->y + t.y,
		this->z + t.z,
		this->w + t.w
	};
}

tuple_t tuple_t::operator-(const tuple_t& t) const
{
	return {
		this->x - t.x,
		this->y - t.y,
		this->z - t.z,
		this->w - t.w
	};
}