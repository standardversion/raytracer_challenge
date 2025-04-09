#pragma once
#include <vector>

struct matrix_t
{
	// MEMBER VARIABLES
	std::vector<std::vector<double>> data;

	// CONSTRUCTORS
	matrix_t(const std::size_t rows, const std::size_t columns);
	matrix_t(const std::vector<std::vector<double>>& input_data);

	// OPERATORS
	double operator()(const std::size_t row, const std::size_t column) const;
	void operator()(const std::size_t row, const std::size_t column, const double value);
};
