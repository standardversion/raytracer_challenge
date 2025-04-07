#pragma once


struct tuple_t
{
	double x;
	double y;
	double z;
	double w;

	static constexpr const double EPSILON{ 0.00001 };

	static tuple_t point(const double x, const double y, const double z);
	static tuple_t vector(const double x, const double y, const double z);
	bool is_point() const;
	bool is_vector() const;
	bool operator==(const tuple_t& t) const;
	tuple_t operator+(const tuple_t& t) const;
	tuple_t operator-(const tuple_t& t) const;
	tuple_t& operator-();
	tuple_t operator-() const;
	tuple_t operator*(const double factor) const;
	tuple_t& operator*=(const double factor);
	tuple_t operator/(const double factor) const;
	tuple_t& operator/=(const double factor);
};