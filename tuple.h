#pragma once

/**
 * @brief Represents a 4-component tuple used for points and vectors in 3D space.
 *
 * This structure uses homogeneous coordinates:
 * - If `w == 1.0`, the tuple represents a **point**.
 * - If `w == 0.0`, it represents a **vector**.
 *
 * Commonly used for geometry, ray tracing, and transformation math.
 */
struct tuple_t
{
	/**
	 * @brief The x-coordinate.
	 */
	double x;

	/**
	 * @brief The y-coordinate.
	 */
	double y;

	/**
	 * @brief The z-coordinate.
	 */
	double z;

	/**
	 * @brief The w-component (1.0 = point, 0.0 = vector).
	 */
	double w;

	/**
	 * @brief Creates a point tuple with the given coordinates.
	 *
	 * This function creates a 3D point represented as a tuple with the specified x, y, and z coordinates.
	 *
	 * @param x The x-coordinate of the point.
	 * @param y The y-coordinate of the point.
	 * @param z The z-coordinate of the point.
	 * @return A tuple_t representing the point in 3D space.
	 */
	static tuple_t point(const double x, const double y, const double z);

	/**
	 * @brief Creates a vector tuple with the given components.
	 *
	 * This function creates a 3D vector represented as a tuple with the specified x, y, and z components.
	 *
	 * @param x The x-component of the vector.
	 * @param y The y-component of the vector.
	 * @param z The z-component of the vector.
	 * @return A tuple_t representing the vector in 3D space.
	 */
	static tuple_t vector(const double x, const double y, const double z);

	/**
	 * @brief Calculates the magnitude (length) of a tuple.
	 *
	 * This function calculates the magnitude (Euclidean length) of a 3D tuple, typically representing either a point or a vector.
	 *
	 * @param t The tuple whose magnitude is to be calculated.
	 * @return The magnitude of the tuple.
	 */
	static double magnitude(const tuple_t& t);

	/**
	 * @brief Normalizes a tuple (scales it to unit length).
	 *
	 * This function normalizes the input tuple by dividing each component by the magnitude of the tuple.
	 * It returns a tuple with the same direction but unit length.
	 *
	 * @param t The tuple to normalize.
	 * @return The normalized tuple.
	 */
	static tuple_t normalize(const tuple_t& t);

	/**
	 * @brief Computes the dot product of two tuples.
	 *
	 * This function calculates the dot product (scalar product) of two 3D tuples.
	 * The dot product is calculated as the sum of the component-wise products of the tuples.
	 *
	 * @param a The first tuple.
	 * @param b The second tuple.
	 * @return The dot product of the two tuples.
	 */
	static double dot(const tuple_t& a, const tuple_t& b);

	/**
	 * @brief Computes the cross product of two vectors.
	 *
	 * This function computes the cross product of two 3D vectors.
	 * The result is a new vector that is perpendicular to both input vectors.
	 *
	 * @param a The first vector.
	 * @param b The second vector.
	 * @return The cross product of the two vectors.
	 */
	static tuple_t cross(const tuple_t& a, const tuple_t& b);

	/**
	 * @brief Checks if the tuple represents a point.
	 *
	 * This function checks whether the tuple represents a point (as opposed to a vector).
	 * Typically, this is determined by whether the tuple's components are used as fixed coordinates.
	 *
	 * @return `true` if the tuple is a point, `false` otherwise.
	 */
	bool is_point() const;

	/**
	 * @brief Checks if the tuple represents a vector.
	 *
	 * This function checks whether the tuple represents a vector (as opposed to a point).
	 * Typically, this is determined by whether the tuple's components are used as directional values.
	 *
	 * @return `true` if the tuple is a vector, `false` otherwise.
	 */
	bool is_vector() const;

	/**
	 * @brief Calculates the magnitude of the tuple.
	 *
	 * This function calculates the magnitude (Euclidean length) of the tuple, which can be used for both points and vectors.
	 *
	 * @return The magnitude of the tuple.
	 */
	double magnitude() const;

	/**
	 * @brief Normalizes the tuple in-place.
	 *
	 * This function normalizes the tuple (scales it to unit length), modifying the tuple directly.
	 *
	 * @note This function changes the tuple to a unit vector or point, depending on the current state.
	 */
	void normalize();

	/**
	 * @brief Computes the reflection vector given a surface normal.
	 *
	 * @param normal The normal vector at the point of reflection.
	 * @return tuple_t The reflected vector, assuming this is the incoming vector.
	 */
	tuple_t reflect(const tuple_t& normal) const;

	/**
	 * @brief Compares two tuples for equality.
	 *
	 * This function checks whether two tuples are equal by comparing their individual components.
	 * Two tuples are considered equal if all corresponding components are equal.
	 *
	 * @param t The tuple to compare against.
	 * @return `true` if the tuples are equal, `false` otherwise.
	 */
	bool operator==(const tuple_t& t) const;

	/**
	 * @brief Adds two tuples component-wise.
	 *
	 * This function performs component-wise addition of two tuples. The result is a new tuple
	 * where each component is the sum of the corresponding components from the two input tuples.
	 *
	 * @param t The tuple to add.
	 * @return A new tuple representing the result of the addition.
	 */
	tuple_t operator+(const tuple_t& t) const;

	/**
	 * @brief Subtracts one tuple from another component-wise.
	 *
	 * This function performs component-wise subtraction of two tuples. The result is a new tuple
	 * where each component is the difference of the corresponding components from the two input tuples.
	 *
	 * @param t The tuple to subtract.
	 * @return A new tuple representing the result of the subtraction.
	 */
	tuple_t operator-(const tuple_t& t) const;

	/**
	 * @brief Negates the tuple in-place.
	 *
	 * This function negates the tuple, i.e., it multiplies each component of the tuple by -1.
	 * The original tuple is modified directly.
	 *
	 * @return A reference to the modified tuple.
	 */
	tuple_t& operator-();

	/**
	 * @brief Negates the tuple and returns a new tuple.
	 *
	 * This function creates and returns a new tuple where each component is the negation of the
	 * corresponding component in the original tuple.
	 *
	 * @return A new tuple with each component negated.
	 */
	tuple_t operator-() const;

	/**
	 * @brief Multiplies the tuple by a scalar.
	 *
	 * This function multiplies each component of the tuple by a scalar factor and returns a new tuple.
	 *
	 * @param factor The scalar factor to multiply each component by.
	 * @return A new tuple representing the scaled tuple.
	 */
	tuple_t operator*(const double factor) const;

	/**
	 * @brief Multiplies the tuple by a scalar in-place.
	 *
	 * This function multiplies each component of the tuple by a scalar factor and modifies the tuple directly.
	 *
	 * @param factor The scalar factor to multiply each component by.
	 * @return A reference to the modified tuple.
	 */
	tuple_t& operator*=(const double factor);

	/**
	 * @brief Divides the tuple by a scalar.
	 *
	 * This function divides each component of the tuple by a scalar factor and returns a new tuple.
	 *
	 * @param factor The scalar factor to divide each component by.
	 * @return A new tuple representing the scaled tuple.
	 */
	tuple_t operator/(const double factor) const;

	/**
	 * @brief Divides the tuple by a scalar in-place.
	 *
	 * This function divides each component of the tuple by a scalar factor and modifies the tuple directly.
	 *
	 * @param factor The scalar factor to divide each component by.
	 * @return A reference to the modified tuple.
	 */
	tuple_t& operator/=(const double factor);

};