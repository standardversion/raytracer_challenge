#include <cmath>
#include <stdexcept>
#include "tuple.h"
#include "settings.h"

// STATIC FUNCTIONS
tuple_t tuple_t::point(const double x, const double y, const double z)
{
	return { x, y, z, 1.0 };
}

tuple_t tuple_t::vector(const double x, const double y, const double z)
{
	return { x, y, z, 0.0 };
}

double tuple_t::magnitude(const tuple_t& t)
{
	if (t.is_point())
	{
		throw std::invalid_argument("Cannot get magnitude of a point");
	}
	return std::sqrtf(std::pow(t.x, 2) + std::pow(t.y, 2) + std::pow(t.z, 2));
}

tuple_t tuple_t::normalize(const tuple_t& t)
{
	if (t.is_point())
	{
		throw std::invalid_argument("Cannot normalize a point");
	}
	const double magnitude{ tuple_t::magnitude(t) };
	return {
		t.x / magnitude,
		t.y / magnitude,
		t.z / magnitude,
		0.0
	};
}

double tuple_t::dot(const tuple_t& a, const tuple_t& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

tuple_t tuple_t::cross(const tuple_t& a, const tuple_t& b)
{
	if (a.is_point() || b.is_point())
	{
		throw std::invalid_argument("Cannot cross a point");
	}
	return tuple_t::vector(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}


// MEMBER FUNCTIONS
bool tuple_t::is_point() const
{
	return w == 1.0 ? true : false;
}

bool tuple_t::is_vector() const
{
	return w == 1.0 ? false : true;
}

double tuple_t::magnitude() const
{
	if (this->is_point())
	{
		throw std::invalid_argument("Cannot get magnitude of a point");
	}
	return std::sqrtf(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
}

void tuple_t::normalize()
{
	if (is_point())
	{
		throw std::invalid_argument("Cannot normalize a point");
	}
	const double mag{ magnitude() };
	x /= mag;
	y /= mag;
	z /= mag;
}
//https://www.contemporarycalculus.com/dh/Calculus_all/CC11_7_VectorReflections.pdf
tuple_t tuple_t::reflect(const tuple_t& normal) const
{
	return *this - normal * 2 * dot(*this, normal);
}

// OPERATORS
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
	if (this->is_vector() && t.is_point())
	{
		throw std::invalid_argument("Cannot subtract point from vector");
	}
	return {
		this->x - t.x,
		this->y - t.y,
		this->z - t.z,
		this->w - t.w
	};
}

tuple_t& tuple_t::operator-()
{
	this->x = -this->x;
	this->y = -this->y;
	this->z = -this->z;
	this->w = -this->w;
	return *this;
}

tuple_t tuple_t::operator-() const
{
	return {
		-this->x,
		-this->y,
		-this->z,
		-this->w
	};
}

tuple_t tuple_t::operator*(const double factor) const
{
	return {
		this->x * factor,
		this->y * factor,
		this->z * factor,
		this->w * factor
	};
}

tuple_t& tuple_t::operator*=(const double factor)
{
	this->x *= factor;
	this->y *= factor;
	this->z *= factor;
	this->w *= factor;
	return *this;
}

tuple_t tuple_t::operator/(const double factor) const
{
	return {
		this->x / factor,
		this->y / factor,
		this->z / factor,
		this->w / factor
	};
}

tuple_t& tuple_t::operator/=(const double factor)
{
	this->x /= factor;
	this->y /= factor;
	this->z /= factor;
	this->w /= factor;
	return *this;
}