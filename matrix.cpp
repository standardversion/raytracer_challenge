#include <stdexcept>
#include "matrix.h"

matrix_t::matrix_t(const std::size_t rows, const std::size_t columns)
{
	if (rows <= 0 || columns <= 0)
	{
		throw std::invalid_argument("Invalid rows/columns");
	}
	data.resize(rows, std::vector<double>(columns));
}

matrix_t::matrix_t(const std::vector<std::vector<double>>& input_data)
{
	const std::size_t rows{ input_data.size() };
	if (rows)
	{
		const std::size_t columns{ input_data[0].size()};
		if (columns)
		{
			for (const auto& row : input_data)
			{
				if (row.size() != columns)
				{
					throw std::invalid_argument("Inconsistent number of columns");
				}
			}
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

// OPERATORS
double matrix_t::operator()(const std::size_t row, const std::size_t column) const
{
	if (row < 0 || row > data.size() || column < 0 || column > data[row].size())
	{
		throw std::invalid_argument("Out of bounds");
	}
	return data[row][column];
}

void matrix_t::operator()(const std::size_t row, const std::size_t column, const double value)
{
	if (row < 0 || row > data.size() || column < 0 || column > data[row].size())
	{
		throw std::invalid_argument("Out of bounds");
	}
	data[row][column] = value;
}