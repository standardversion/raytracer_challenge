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

// MEMBER FUNCTIONS
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
				if (xy == 0)
				{
					det += data[x][y] * minors[xy];
				}
				else if (xy % 2)
				{
					det += data[x][y] * -minors[xy];
				}
				else
				{
					det += data[x][y] * minors[xy];
				}
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
			if (xy == 0)
			{
				co_factors.data[x][y] = minor;
			}
			else if (xy % 2)
			{
				co_factors.data[x][y] = -minor;
			}
			else
			{
				co_factors.data[x][y] = minor;
			}
		}
	}
	matrix_t co_factor_transpose{ co_factors.transpose() };
	matrix_t inverse{ co_factor_transpose / det };
	return inverse;
}

// OPERATORS
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