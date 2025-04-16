#pragma once
#include <vector>
#include "tuple.h"

/**
 * @brief Represents a 2D matrix of double-precision values.
 *
 * Used for performing linear algebra operations such as transformations,
 * matrix multiplication, and inversion — commonly in graphics and ray tracing.
 */
struct matrix_t
{
	/**
	 * @brief The number of rows in the matrix.
	 */
	std::size_t rows;

	/**
	 * @brief The number of columns in the matrix.
	 */
	std::size_t columns;

	/**
	 * @brief The matrix data stored as a 2D vector of doubles.
	 *
	 * Indexed as data[row][column]. Each inner vector represents a row.
	 */
	std::vector<std::vector<double>> data;

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

	/**
	 * @brief Creates an identity matrix.
	 * | 1  0  0  0 |
	 * | 0  1  0  0 |
	 * | 0  0  1  0 |
	 * | 0  0  0  1 |
	 * @param r Number of rows (default is 4).
	 * @param c Number of columns (default is 4).
	 * @return A matrix_t representing the identity matrix of size r x c.
	 */
	static matrix_t identity(const std::size_t r = 4, const std::size_t c = 4);

	/**
	 * @brief Creates a translation matrix.
	 * | 1  0  0  tx |
	 * | 0  1  0  ty |
	 * | 0  0  1  tz |
	 * | 0  0  0  1  |
	 * @param tx Translation distance along the X-axis.
	 * @param ty Translation distance along the Y-axis.
	 * @param tz Translation distance along the Z-axis.
	 * @return A matrix_t representing the translation transformation.
	 */
	static matrix_t translation(const double tx, const double ty, const double tz);

	/**
	 * @brief Creates a scaling matrix.
	 * | sx  0  0  0 |
	 * | 0  sy  0  0 |
	 * | 0  0  sz  0 |
	 * | 0  0  0   1 |
	 * @param sx Scaling factor along the X-axis.
	 * @param sy Scaling factor along the Y-axis.
	 * @param sz Scaling factor along the Z-axis.
	 * @return A matrix_t representing the scaling transformation.
	 */
	static matrix_t scaling(const double sx, const double sy, const double sz);

	/**
	 * @brief Creates a rotation matrix around the X-axis.
	 * | 1  0  0  0 |
	 * | 0  c -s  0 |
	 * | 0  s  c  0 |
	 * | 0  0  0  1 |
	 * @param radians Rotation angle in radians.
	 * @return A matrix_t representing the rotation around the X-axis.
	 */
	static matrix_t rotation_x(const double radians);

	/**
	 * @brief Creates a rotation matrix around the Y-axis.
	 * |  c  0  s  0 |
	 * |  0  1  0  0 |
	 * | -s  0  c  0 |
	 * |  0  0  0  1 |
	 * @param radians Rotation angle in radians.
	 * @return A matrix_t representing the rotation around the Y-axis.
	 */
	static matrix_t rotation_y(const double radians);

	/**
	 * @brief Creates a rotation matrix around the Z-axis.
	 * | c -s  0  0 |
	 * | s  c  0  0 |
	 * | 0  0  1  0 |
	 * | 0  0  0  1 |
	 * @param radians Rotation angle in radians.
	 * @return A matrix_t representing the rotation around the Z-axis.
	 */
	static matrix_t rotation_z(const double radians);

	/**
	 * @brief Creates a shearing transformation matrix.
	 * | 1  Xy Xz  0 |
	 * | Yx  1 Yz  0 |
	 * | Zx  Zy 1  0 |
	 * | 0  0  0   1 |
	 * @param Xy Shear factor of X in proportion to Y.
	 * @param Xz Shear factor of X in proportion to Z.
	 * @param Yx Shear factor of Y in proportion to X.
	 * @param Yz Shear factor of Y in proportion to Z.
	 * @param Zx Shear factor of Z in proportion to X.
	 * @param Zy Shear factor of Z in proportion to Y.
	 * @return A matrix_t representing the shearing transformation.
	 */
	static matrix_t shearing(const double Xy, const double Xz, const double Yx, const double Yz, const double Zx, const double Zy);

	/**
	 * @brief Computes a view transformation matrix from world space to camera space.
	 *
	 * @param from The point in world space where the camera is located (the eye or observer position).
	 * @param to The point in world space the camera is looking at (the target).
	 * @param up The up direction vector, indicating which way is "up" for the camera.
	 * @return A transformation matrix that converts world coordinates into the camera's view space.
	 *
	 * This function builds a view transformation (also known as a "look-at" matrix)
	 * by computing the orientation of the camera based on the `from`, `to`, and `up` vectors.
	 * It is typically used to position and orient a camera within a 3D scene.
	 */
	static matrix_t view_transform(const tuple_t& from, const tuple_t& to, const tuple_t& up);


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
