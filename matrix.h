#pragma once
#include <vector>
#include "tuple.h"

struct matrix_t
{
	// MEMBER VARIABLES
	std::size_t rows;
	std::size_t columns;
	std::vector<std::vector<double>> data;

	// CONSTRUCTORS
	/**
	 * @brief Constructs a matrix with the given number of rows and columns, initialized to zero.
	 *
	 * This constructor creates a matrix of size r × c with all elements initialized to 0.
	 *
	 * @param r The number of rows.
	 * @param c The number of columns.
	 */
	matrix_t(const std::size_t r, const std::size_t c);

	/**
	 * @brief Constructs a matrix from a 2D vector of doubles.
	 *
	 * This constructor builds a matrix using the provided 2D vector of data.
	 * All inner vectors must have the same size, representing the number of columns.
	 *
	 * @param input_data A 2D vector representing the matrix values.
	 */
	matrix_t(const std::vector<std::vector<double>>& input_data);

	/**
	 * @brief Constructs a column matrix from a tuple.
	 *
	 * This constructor converts a tuple (e.g., a vector or point) into a column matrix.
	 *
	 * @param t A tuple_t object representing a vector.
	 */
	matrix_t(const tuple_t& t);


	// MEMBER FUNCTIONS
	/**
	 * @brief Returns the transpose of the matrix.
	 *
	 * This function swaps the rows and columns of the matrix.
	 *
	 * @return A transposed matrix.
	 */
	matrix_t transpose() const;

	/**
	 * @brief Returns a submatrix excluding a specific row and column.
	 *
	 * This is used in calculating cofactors and determinants.
	 *
	 * @param r The row to exclude.
	 * @param c The column to exclude.
	 * @return A submatrix with the specified row and column removed.
	 */
	matrix_t sub_matrix(const std::size_t r, const std::size_t c) const;

	/**
	 * @brief Calculates the determinant of the matrix.
	 *
	 * Only defined for square matrices. Throws an exception otherwise.
	 *
	 * @return The determinant as a double.
	 * @throws std::invalid_argument If the matrix is not square.
	 */
	double determinant() const;

	/**
	 * @brief Calculates the inverse of the matrix.
	 *
	 * This function returns the inverse of the matrix using the adjugate and determinant.
	 *
	 * @return The inverse matrix.
	 * @throws std::invalid_argument If the matrix is not square or not invertible.
	 */
	matrix_t inverse() const;


	// OPERATORS
	/**
	 * @brief Accesses a matrix element by row and column.
	 *
	 * Retrieves the value at the specified row and column index.
	 *
	 * @param row The row index (0-based).
	 * @param column The column index (0-based).
	 * @return The value at the specified position.
	 */
	double operator()(const std::size_t row, const std::size_t column) const;

	/**
	 * @brief Sets a matrix element at a specific row and column.
	 *
	 * Updates the value at the specified location in the matrix.
	 *
	 * @param row The row index (0-based).
	 * @param column The column index (0-based).
	 * @param value The value to set at the given position.
	 */
	void operator()(const std::size_t row, const std::size_t column, const double value);

	/**
	 * @brief Compares two matrices for equality.
	 *
	 * Returns true if all corresponding elements are equal.
	 *
	 * @param m The matrix to compare with.
	 * @return True if the matrices are equal, false otherwise.
	 */
	bool operator==(const matrix_t& m) const;

	/**
	 * @brief Multiplies this matrix with another matrix.
	 *
	 * Performs matrix multiplication. The number of columns in the left matrix must match
	 * the number of rows in the right matrix.
	 *
	 * @param m The matrix to multiply with.
	 * @return The resulting product matrix.
	 * @throws std::invalid_argument If the dimensions do not match.
	 */
	matrix_t operator*(const matrix_t& m) const;

	/**
	 * @brief Multiplies this matrix with a tuple (vector).
	 *
	 * Interprets the tuple as a column vector and performs matrix-vector multiplication.
	 *
	 * @param t The tuple (vector) to multiply with.
	 * @return A tuple_t representing the result of the multiplication.
	 * @throws std::invalid_argument If the matrix and tuple dimensions do not match.
	 */
	tuple_t operator*(const tuple_t& t) const;

	/**
	 * @brief Divides all elements of the matrix by a scalar factor.
	 *
	 * Each element of the matrix is divided by the given factor.
	 *
	 * @param factor The scalar divisor.
	 * @return The resulting matrix after division.
	 */
	matrix_t operator/(const double factor) const;

};
