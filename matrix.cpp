#include <cmath>
#include <stdexcept>
#include "matrix.h"
#include "settings.h"

matrix_t::matrix_t(const std::size_t r, const std::size_t c)
{
	if (r <= 0 || c <= 0)
	{
		throw std::invalid_argument("Invalid rows/columns");
	}
	rows = r;
	columns = c;
	data.resize(rows, std::vector<double>(columns));
}

matrix_t::matrix_t(const std::vector<std::vector<double>>& input_data)
{
	const std::size_t r{ input_data.size() };
	if (r)
	{
		const std::size_t c{ input_data[0].size()};
		if (c)
		{
			for (const auto& row : input_data)
			{
				if (row.size() != c)
				{
					throw std::invalid_argument("Inconsistent number of columns");
				}
			}
			rows = r;
			columns = c;
			data = input_data;
		}
		else
		{
			throw std::invalid_argument("Input data cannot be empty");
		}
	}
	else
	{
		throw std::invalid_argument("Input data cannot be empty");
	}
}

matrix_t::matrix_t(const tuple_t& t)
	: rows{ 4 }, columns{ 1 }
{
	data.resize(4, std::vector<double>(1));
	data[0][0] = t.x;
	data[1][0] = t.y;
	data[2][0] = t.z;
	data[3][0] = t.w;
}


// | 1  0  0  0 |
// | 0  1  0  0 |
// | 0  0  1  0 |
// | 0  0  0  1 |
matrix_t matrix_t::identity(const std::size_t r, const std::size_t c)
{
	matrix_t i{ 4, 4 };
	i.data[0][0] = 1;
	i.data[1][1] = 1;
	i.data[2][2] = 1;
	i.data[3][3] = 1;
	return i;
}

// | 1  0  0  tx |
// | 0  1  0  ty |
// | 0  0  1  tz |
// | 0  0  0  1  |
matrix_t matrix_t::translation(const double tx, const double ty, const double tz)
{
	matrix_t t{ 4, 4 };
	t.data[0][0] = 1;
	t.data[1][1] = 1;
	t.data[2][2] = 1;
	t.data[3][3] = 1;
	t.data[0][3] = tx;
	t.data[1][3] = ty;
	t.data[2][3] = tz;
	return t;
}

// | sx  0  0  0 |
// |  0 sy  0  0 |
// |  0  0 sz  0 |
// |  0  0  0  1 |
matrix_t matrix_t::scaling(const double sx, const double sy, const double sz)
{
	matrix_t s{ 4, 4 };
	s.data[0][0] = sx;
	s.data[1][1] = sy;
	s.data[2][2] = sz;
	s.data[3][3] = 1;
	return s;
}

// | 1  0  0  0 |
// | 0  c -s  0 |
// | 0  s  c  0 |
// | 0  0  0  1 |
matrix_t matrix_t::rotation_x(const double radians)
{
	matrix_t rx{ 4, 4 };
	rx.data[0][0] = 1;
	rx.data[1][1] = cos(radians);
	rx.data[1][2] = -sin(radians);
	rx.data[2][1] = sin(radians);
	rx.data[2][2] = cos(radians);
	rx.data[3][3] = 1;
	return rx;
}

// |  c  0  s  0 |
// |  0  1  0  0 |
// | -s  0  c  0 |
// |  0  0  0  1 |
matrix_t matrix_t::rotation_y(const double radians)
{
	matrix_t ry{ 4, 4 };
	ry.data[0][0] = cos(radians);
	ry.data[0][2] = sin(radians);
	ry.data[1][1] = 1;
	ry.data[2][0] = -sin(radians);
	ry.data[2][2] = cos(radians);
	ry.data[3][3] = 1;
	return ry;
}

// | c -s  0  0 |
// | s  c  0  0 |
// | 0  0  1  0 |
// | 0  0  0  1 |
matrix_t matrix_t::rotation_z(const double radians)
{
	matrix_t rz{ 4, 4 };
	rz.data[0][0] = cos(radians);
	rz.data[0][1] = -sin(radians);
	rz.data[1][0] = sin(radians);
	rz.data[1][1] = cos(radians);
	rz.data[2][2] = 1;
	rz.data[3][3] = 1;
	return rz;
}

// | 1  Xy Xz  0 |
// | Yx  1 Yz  0 |
// | Zx  Zy 1  0 |
// | 0  0  0   1 |
matrix_t matrix_t::shearing(const double Xy, const double Xz, const double Yx, const double Yz, const double Zx, const double Zy)
{
	matrix_t s{ 4, 4 };
	s.data[0][0] = 1;
	s.data[0][1] = Xy;
	s.data[0][2] = Xz;
	s.data[1][0] = Yx;
	s.data[1][1] = 1;
	s.data[1][2] = Yz;
	s.data[2][0] = Zx;
	s.data[2][1] = Zy;
	s.data[2][2] = 1;
	s.data[3][3] = 1;
	return s;
}

matrix_t matrix_t::view_transform(const tuple_t& from, const tuple_t& to, const tuple_t& up)
{
	tuple_t forward{ to - from };
	forward.normalize();
	tuple_t normalized_up{ up };
	normalized_up.normalize();
	const tuple_t left{ tuple_t::cross(forward, normalized_up) };
	const tuple_t true_up{ tuple_t::cross(left, forward) };
	const std::vector<std::vector<double>> vecs{
		{ left.x, left.y, left.z, 0 },
		{ true_up.x, true_up.y, true_up.z, 0 },
		{ -forward.x, -forward.y, -forward.z, 0.00000 },
		{ 0.00000, 0.00000, 0.00000, 1.00000 }
	};
	const matrix_t orientation{ vecs };
	return orientation * translation(-from.x, -from.y, -from.z);
}

matrix_t matrix_t::transpose() const
{
	matrix_t r{ rows, columns };
	for (std::size_t x{ 0 }; x < rows; x++)
	{
		for (std::size_t y{ 0 }; y < columns; y++)
		{
			r.data[x][y] = data[y][x];
		}
	}
	return r;
}

matrix_t matrix_t::sub_matrix(const std::size_t r, const std::size_t c) const
{
	matrix_t sub{ rows - 1, columns - 1 };
	std::size_t sub_r{ 0 };
	for (std::size_t x{ 0 }; x < rows; x++)
	{
		if (x == r)
		{
			continue;
		}
		std::size_t sub_c{ 0 };
		for (std::size_t y{ 0 }; y < columns; y++)
		{
			if (y == c)
			{
				continue;
			}
			sub.data[sub_r][sub_c] = data[x][y];
			sub_c++;
		}
		sub_r++;
	}
	return sub;
}


double matrix_t::determinant() const
{
	if (rows != columns)
	{
		std::invalid_argument("Determinant of a non square matrix is undefined");
	}
	if (rows == 2)
	{
		/*
		* Determinant of a 2x2 matrix
		* M = | a b |
		*	  | c d |
		* |M| = ad - bc
		*/
		return data[0][0] * data[1][1] - data[0][1] * data[1][0];
	}
	else {
		std::vector<double> minors{};
		for (std::size_t x{ 0 }; x < 1; x++)
		{
			for (std::size_t y{ 0 }; y < columns; y++)
			{
				matrix_t sub_m{ sub_matrix(x, y) };
				double minor{ sub_m.determinant() };
				minors.push_back(minor);
			}
		}
		double det{ 0 };
		for (std::size_t x{ 0 }; x < 1; x++)
		{
			for (std::size_t y{ 0 }; y < columns; y++)
			{
				std::size_t xy{ x + y };
				double minor{ minors[xy] };
				// co-factors alternate between +ve & -ve
				minor = xy % 2 ? (xy == 0 ? minor : -minor) : minor;
				det += data[x][y] * minor;
			}
		}
		return det;
	}
}

matrix_t matrix_t::inverse() const
{
	if (rows != columns)
	{
		std::invalid_argument("Cannot calculate inverse of a non square matrix");
	}
	const double det{ determinant() };
	if (det < 0)
	{
		std::invalid_argument("Cannot calculate inverse of a matrix whose determinant is 0");
	}
	matrix_t co_factors{ rows, columns };
	for (std::size_t x{ 0 }; x < rows; x++)
	{
		for (std::size_t y{ 0 }; y < columns; y++)
		{
			matrix_t sub_m{ sub_matrix(x, y) };
			double minor{ sub_m.determinant() };
			std::size_t xy{ x + y };
			// co-factors alternate between +ve & -ve
			minor = xy % 2 ? (xy == 0 ? minor : -minor) : minor;
			// set [y][x] instead of [x][y] so we create a transposed matrix directly
			// then we can skip the transpose step
			co_factors.data[y][x] = minor;
		}
	}
	matrix_t inverse{ co_factors / det };
	return inverse;
}

double matrix_t::operator()(const std::size_t row, const std::size_t column) const
{
	if (row < 0 || row > rows || column < 0 || column > columns)
	{
		throw std::invalid_argument("Out of bounds");
	}
	return data[row][column];
}

void matrix_t::operator()(const std::size_t row, const std::size_t column, const double value)
{
	if (row < 0 || row > rows || column < 0 || column > columns)
	{
		throw std::invalid_argument("Out of bounds");
	}
	data[row][column] = value;
}

bool matrix_t::operator==(const matrix_t& m) const
{
	if (rows != m.rows || columns != m.columns)
	{
		return false;
	}
	else
	{
		for (std::size_t x{ 0 }; x < rows; x++)
		{
			for (std::size_t y{ 0 }; y < columns; y++)
			{
				const double diff{ std::fabs(data[x][y] - m.data[x][y]) };
				if (diff > EPSILON)
				{
					return false;
				}
			}
		}
	}
	return true;
}

matrix_t matrix_t::operator*(const matrix_t& m) const
{
	if (columns != m.rows)
	{
		throw std::invalid_argument("Cannot multiply a matrix. Columns, rows mismatch");
	}
	matrix_t r{ rows, m.columns };
	for (std::size_t x{ 0 }; x < rows; x++)
	{
		for (std::size_t y{ 0 }; y < m.columns; y++)
		{
			for (std::size_t k{ 0 }; k < columns; k++)
			{
				r.data[x][y] += data[x][k] * m.data[k][y];
			}
		}
	}
	return r;
}

tuple_t matrix_t::operator*(const tuple_t& t) const
{
	const matrix_t tuple_to_matrix{ t };
	const matrix_t result{*this * tuple_to_matrix };
	return { result.data[0][0], result.data[1][0], result.data[2][0], result.data[3][0] };
}

matrix_t matrix_t::operator/(const double factor) const
{
	matrix_t r{ rows, columns };
	for (std::size_t x{ 0 }; x < rows; x++)
	{
		for (std::size_t y{ 0 }; y < columns; y++)
		{
			r.data[x][y] = data[x][y] / factor;
		}
	}
	return r;
}