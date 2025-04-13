#include <cmath>
#include "gtest/gtest.h"
#include "../tuple.h"


/*
Scenario: A tuple with w=1.0 is a point
  Given a ← tuple(4.3, -4.2, 3.1, 1.0)
  Then a.x = 4.3
	And a.y = -4.2
	And a.z = 3.1
	And a.w = 1.0
	And a is a point
	And a is not a vector
*/
TEST(tuple, should_be_a_point_if_w_is_1_dot_0)
{
	const tuple_t a{ 4.3, -4.2, 3.1, 1.0 };
	EXPECT_DOUBLE_EQ(a.x, 4.3);
	EXPECT_DOUBLE_EQ(a.y, -4.2);
	EXPECT_DOUBLE_EQ(a.z, 3.1);
	EXPECT_DOUBLE_EQ(a.w, 1.0);
	EXPECT_TRUE(a.is_point());
	EXPECT_FALSE(a.is_vector());
}

/*
Scenario: A tuple with w=0 is a vector
  Given a ← tuple(4.3, -4.2, 3.1, 0.0)
  Then a.x = 4.3
	And a.y = -4.2
	And a.z = 3.1
	And a.w = 0.0
	And a is not a point
	And a is a vector
*/
TEST(tuple, should_be_a_vector_if_w_is_0)
{
	const tuple_t a{ 4.3, -4.2, 3.1, 0.0 };
	EXPECT_DOUBLE_EQ(a.x, 4.3);
	EXPECT_DOUBLE_EQ(a.y, -4.2);
	EXPECT_DOUBLE_EQ(a.z, 3.1);
	EXPECT_DOUBLE_EQ(a.w, 0.0);
	EXPECT_FALSE(a.is_point());
	EXPECT_TRUE(a.is_vector());
}

/*
Scenario: point() creates tuples with w=1
  Given p ← point(4, -4, 3)
  Then p = tuple(4, -4, 3, 1)
*/
TEST(tuple, should_create_a_tuple_using_point_static_func)
{
	const tuple_t p_point{ tuple_t::point(4, -4, 3) };
	const tuple_t p_tuple{ 4, -4, 3, 1.0 };
	EXPECT_EQ(p_point.x, p_tuple.x);
	EXPECT_EQ(p_point.y, p_tuple.y);
	EXPECT_EQ(p_point.z, p_tuple.z);
	EXPECT_EQ(p_point.w, p_tuple.w);
}

/*
Scenario: vector() creates tuples with w=0
  Given v ← vector(4, -4, 3)
  Then v = tuple(4, -4, 3, 0)
*/
TEST(tuple, should_create_a_tuple_using_vector_static_func)
{
	const tuple_t v_vector{ tuple_t::vector(4, -4, 3) };
	const tuple_t v_tuple{ 4, -4, 3, 0.0 };
	EXPECT_EQ(v_vector.x, v_tuple.x);
	EXPECT_EQ(v_vector.y, v_tuple.y);
	EXPECT_EQ(v_vector.z, v_tuple.z);
	EXPECT_EQ(v_vector.w, v_tuple.w);
}

/*
Scenario: Comparing two tuples
  Given a ← tuple(4.3, -4.2, 3.1, 0.0)
  And b ← tuple(4.3, -4.2, 3.1, 1.0)
  And c ← tuple(4.3, -4.2, 3.1, 0.0)
  Then a != b
  And a = c
*/
TEST(tuple, should_implement_comparison_operator)
{
	const tuple_t a{ 4.3, -4.2, 3.1, 0.0 };
	const tuple_t b{ 4.3, -4.2, 3.1, 1.0 };
	const tuple_t c{ 4.3, -4.2, 3.1, 0.0 };
	EXPECT_FALSE(a == b);
	EXPECT_TRUE(a == c);
}

/*
Scenario: Adding two tuples
  Given a ← tuple(3, -2, 5, 1)
	And b ← tuple(-2, 3, 1, 0)
   Then a + ab = tuple(1, 1, 6, 1)
*/
TEST(tuple, should_implement_addition_operator)
{
	const tuple_t a{ 3, -2, 5, 1 };
	const tuple_t b{ -2, 3, 1, 0 };
	const tuple_t c{ a + b };
	const tuple_t expected{ 1, 1, 6, 1 };
	EXPECT_EQ(c, expected);
}

/*
Scenario: Adding two points
  Given a1 ← tuple(3, -2, 5, 1)
	And a2 ← tuple(-2, 3, 1, 1)
   Then a1 + a2 throws invalid argument exception
*/
TEST(tuple, should_throw_invalid_arg_when_adding_two_points)
{
	const tuple_t a{ 3, -2, 5, 1 };
	const tuple_t b{ -2, 3, 1, 1 };
	EXPECT_THROW(a + b, std::invalid_argument);
}

/*
Scenario: Subtracting two points
  Given p1 ← point(3, 2, 1)
	And p2 ← point(5, 6, 7)
  Then p1 - p2 = vector(-2, -4, -6)
*/
TEST(tuple, should_implement_subtraction_operator_two_points_gives_a_vector)
{
	const tuple_t p1{ tuple_t::point(3, 2, 1) };
	const tuple_t p2{ tuple_t::point(5, 6, 7) };
	const tuple_t v{ tuple_t::vector(-2, -4, -6) };
	EXPECT_EQ(p1 - p2, v);
	EXPECT_TRUE(v.is_vector());
}

/*
Scenario: Subtracting a vector from a point
  Given p ← point(3, 2, 1)
   And v ← vector(5, 6, 7)
  Then p - v = point(-2, -4, -6)
*/
TEST(tuple, should_result_is_a_point_when_vector_is_subtracted_from_a_point)
{
	const tuple_t p{ tuple_t::point(3, 2, 1) };
	const tuple_t v{ tuple_t::vector(5, 6, 7) };
	const tuple_t r{ tuple_t::point(-2, -4, -6) };
	EXPECT_EQ(p - v, r);
	EXPECT_TRUE(r.is_point());
}

/*
Scenario: Subtracting two vectors
  Given v1 ← vector(3, 2, 1)
	And v2 ← vector(5, 6, 7)
  Then v1 - v2 = vector(-2, -4, -6)
*/
TEST(tuple, should_result_is_a_vector_when_two_vectors_are_subtracted)
{
	const tuple_t v1{ tuple_t::vector(3, 2, 1) };
	const tuple_t v2{ tuple_t::vector(5, 6, 7) };
	const tuple_t r{ tuple_t::vector(-2, -4, -6) };
	EXPECT_EQ(v1 - v2, r);
	EXPECT_TRUE(r.is_vector());
}

/*
Scenario: Subtracting a point from a vector
  Given v ← vector(3, 2, 1)
	And p ← point(3, 2, 1)
   Then v - p throws invalid argument exception
*/
TEST(tuple, should_throw_invalid_arg_when_subtracting_point_from_vector)
{
	const tuple_t v{ tuple_t::vector(3, 2, 1) };
	const tuple_t p{ tuple_t::point(3, 2, 1) };
	EXPECT_THROW(v - p, std::invalid_argument);
}

/*
Scenario: Subtracting a vector from the zero vector
  Given zero ← vector(0, 0, 0)
	And v ← vector(1, -2, 3)
  Then zero - v = vector(-1, 2, -3)
*/
TEST(tuple, should_subtract_a_vector_from_the_zero_vector)
{
	const tuple_t zero{ tuple_t::vector(0, 0, 0) };
	const tuple_t v{ tuple_t::vector(1, -2, 3) };
	const tuple_t r{ tuple_t::vector(-1, 2, -3) };
	EXPECT_EQ(zero - v, r);
}

/*
Scenario: Negating a tuple in place
  Given a ← tuple(1, -2, 3, -4)
  Then -a = tuple(-1, 2, -3, 4)
*/
TEST(tuple, should_be_able_to_negate_a_tuple_in_place)
{
	tuple_t a{ 1, -2, 3, -4 };
	const tuple_t r{ -1, 2, -3, 4 };
	EXPECT_EQ(-a, r);
}

/*
Scenario: Negating a tuple as a copy
  Given a ← tuple(1, -2, 3, -4)
	And r = -a
  Then r = tuple(-1, 2, -3, 4)
*/
TEST(tuple, should_be_able_to_negate_a_tuple_as_a_copy)
{
	const tuple_t a{ 1, -2, 3, -4 };
	const tuple_t r{ -a };
	const tuple_t expected{ -1, 2, -3, 4 };
	EXPECT_EQ(r, expected);
}

/*
Scenario: Multiplying a tuple by a scalar returns a new tuple
  Given a ← tuple(1, -2, 3, -4)
  Then a * 3.5 = tuple(3.5, -7, 10.5, -14)
*/
TEST(tuple, should_multiply_a_tuple_by_a_scalar_as_a_copy)
{
	const tuple_t a{ 1, -2, 3, -4 };
	const tuple_t r{ 3.5, -7, 10.5, -14 };
	EXPECT_EQ(a * 3.5, r);
}

/*
Scenario: Multiplying a tuple by a scalar modifies the tuple
  Given a ← tuple(1, -2, 3, -4)
	And a *= 3.5
  Then a = tuple(3.5, -7, 10.5, -14)
*/
TEST(tuple, should_multiply_a_tuple_by_a_scalar_in_place)
{
	tuple_t a{ 1, -2, 3, -4 };
	const tuple_t r{ 3.5, -7, 10.5, -14 };
	EXPECT_EQ(a *= 3.5, r);
}

/*
Scenario: Multiplying a tuple by a fraction returns a new tuple
  Given a ← tuple(1, -2, 3, -4)
  Then a * 0.5 = tuple(0.5, -1, 1.5, -2)
*/
TEST(tuple, should_multiply_a_tuple_by_a_fraction_as_a_copy)
{
	const tuple_t a{ 1, -2, 3, -4 };
	const tuple_t r{ 0.5, -1, 1.5, -2 };
	EXPECT_EQ(a * 0.5, r);
}

/*
Scenario: Multiplying a tuple by a fraction modifies the tuple
  Given a ← tuple(1, -2, 3, -4)
	And a *= 0.5
  Then a = tuple(0.5, -1, 1.5, -2)
*/
TEST(tuple, should_multiply_a_tuple_by_a_fraction_in_place)
{
	tuple_t a{ 1, -2, 3, -4 };
	const tuple_t r{ 0.5, -1, 1.5, -2 };
	EXPECT_EQ(a *= 0.5, r);
}

/*
Scenario: Dividing a tuple by a scalar returns a new tuple
  Given a ← tuple(1, -2, 3, -4)
  Then a / 2 = tuple(0.5, -1, 1.5, -2)
*/
TEST(tuple, should_divide_a_tuple_by_a_scalar_as_a_copy)
{
	const tuple_t a{ 1, -2, 3, -4 };
	const tuple_t r{ 0.5, -1, 1.5, -2 };
	EXPECT_EQ(a / 2, r);
}

/*
Scenario: Dividing a tuple by a scalar in place
  Given a ← tuple(1, -2, 3, -4)
	And a /= 2
  Then a = tuple(0.5, -1, 1.5, -2)
*/
TEST(tuple, should_divide_a_tuple_by_a_scalar_in_place)
{
	tuple_t a{ 1, -2, 3, -4 };
	const tuple_t r{ 0.5, -1, 1.5, -2 };
	EXPECT_EQ(a /= 2, r);
}

/*
Scenario: Computing the magnitude of vector(1, 0, 0)
  Given v ← vector(1, 0, 0)
  Then magnitude(v) = 1
*/
TEST(tuple, should_calculate_magnitude_of_a_vector_static)
{
	const tuple_t v{ tuple_t::vector(1, 0, 0) };
	const double m{ tuple_t::magnitude(v) };
	EXPECT_EQ(m, 1);
}

/*
Scenario: Computing the magnitude of point(1, 0, 0)
  Given p ← point(1, 0, 0)
  Then magnitude(p) throws invalid argument exception
*/
TEST(tuple, should_throw_invalid_arg_when_getting_magnitude_of_a_point_static)
{
	const tuple_t p{ tuple_t::point(1, 0, 0) };
	EXPECT_THROW(tuple_t::magnitude(p), std::invalid_argument);
}

/*
Scenario: Computing the magnitude of vector(0, 1, 0)
  Given v ← vector(0, 1, 0)
  Then v.magnitude() = 1
*/
TEST(tuple, should_calculate_magnitude_of_a_vector)
{
	const tuple_t v{ tuple_t::vector(0, 1, 0) };
	const double m{ v.magnitude() };
	EXPECT_EQ(m, 1);
}

/*
Scenario: Computing the magnitude of point(0, 1, 0)
  Given p ← point(0, 1, 0)
  Then p.magnitude() throws invalid argument exception
*/
TEST(tuple, should_throw_invalid_arg_when_getting_magnitude_of_a_point)
{
	const tuple_t p{ tuple_t::point(1, 0, 0) };
	EXPECT_THROW(p.magnitude(), std::invalid_argument);
}

/*
Scenario: Computing the magnitude of vector(0, 0, 1)
  Given v ← vector(0, 0, 1)
  Then magnitude(v) = 1
*/
TEST(tuple, should_calculate_magnitude_of_a_vector_static_2)
{
	const tuple_t v{ tuple_t::vector(0, 0, 1) };
	const double m{ tuple_t::magnitude(v) };
	EXPECT_EQ(m, 1);
}

/*
Scenario: Computing the magnitude of vector(1, 2, 3)
  Given v ← vector(1, 2, 3)
  Then magnitude(v) = √14
*/
TEST(tuple, should_calculate_magnitude_of_a_vector_static_3)
{
	const tuple_t v{ tuple_t::vector(1, 2, 3) };
	const double m{ tuple_t::magnitude(v) };
	EXPECT_EQ(m, std::sqrtf(14));
}

/*
Scenario: Computing the magnitude of vector(-1, -2, -3)
  Given v ← vector(-1, -2, -3)
  Then v.magnitude() = √14
*/
TEST(tuple, should_calculate_magnitude_of_a_vector_static_4)
{
	const tuple_t v{ tuple_t::vector(-1, -2, -3) };
	const double m{ v.magnitude() };
	EXPECT_EQ(m, std::sqrtf(14));
}

/*
Scenario: Normalizing vector(4, 0, 0) gives (1, 0, 0)
  Given v ← vector(4, 0, 0)
  Then normalize(v) = vector(1, 0, 0)
*/
TEST(tuple, should_normalize_a_vector_static)
{
	const tuple_t v{ tuple_t::vector(4, 0, 0) };
	const tuple_t n{ tuple_t::normalize(v) };
	const tuple_t expected{ 1, 0, 0 };
	EXPECT_EQ(n, expected);
}

/*
Scenario: Normalizing a point(4, 0, 0)
  Given p ← point(4, 0, 0)
  Then normalize(p) throws invalid argument exception
*/
TEST(tuple, should_throw_invalid_arg_when_normalizing_a_point_static)
{
	const tuple_t p{ tuple_t::point(1, 0, 0) };
	EXPECT_THROW(tuple_t::normalize(p), std::invalid_argument);
}

/*
Scenario: Normalizing vector(1, 2, 3)
  Given v ← vector(1, 2, 3)
								  # vector(1/√14,   2/√14,   3/√14)
  Then v.normalize() = approximately vector(0.26726, 0.53452, 0.80178)
*/
TEST(tuple, should_normalize_a_vector)
{
	tuple_t v{ tuple_t::vector(1, 2, 3) };
	v.normalize();
	const tuple_t expected{ 0.26726, 0.53452, 0.80178 };
	EXPECT_EQ(v, expected);
}

/*
Scenario: Normalizing a point(1, 2, 3)
  Given p ← point(1, 2, 3)
  Then p.normalize() throws invalid argument exception
*/
TEST(tuple, should_throw_invalid_arg_when_normalizing_a_point)
{
	tuple_t p{ tuple_t::point(1, 2, 3) };
	EXPECT_THROW(p.normalize(), std::invalid_argument);
}

/*
Scenario: The magnitude of a normalized vector
  Given v ← vector(1, 2, 3)
  When norm ← normalize(v)
  Then magnitude(norm) = 1
*/
TEST(tuple, should_get_magnitude_of_a_normalized_vector)
{
	const tuple_t v{ tuple_t::vector(1, 2, 3) };
	const tuple_t norm{ tuple_t::normalize(v) };
	const double magnitude{ tuple_t::magnitude(norm) };
	EXPECT_NEAR(magnitude, 1, 0.00001);
}

/*
Scenario: The dot product of two tuples
  Given a ← vector(1, 2, 3)
	And b ← vector(2, 3, 4)
  Then dot(a, b) = 20
*/
TEST(tuple, should_get_dot_product_of_two_vectors)
{
	const tuple_t a{ tuple_t::vector(1, 2, 3) };
	const tuple_t b{ tuple_t::vector(2, 3, 4) };
	const double r{ tuple_t::dot(a, b) };
	EXPECT_EQ(r, 20);
}

/*
Scenario: The cross product of two vectors
  Given a ← vector(1, 2, 3)
	And b ← vector(2, 3, 4)
  Then cross(a, b) = vector(-1, 2, -1)
	And cross(b, a) = vector(1, -2, 1)
*/
TEST(tuple, should_get_cross_product_of_two_vectors)
{
	const tuple_t a{ tuple_t::vector(1, 2, 3) };
	const tuple_t b{ tuple_t::vector(2, 3, 4) };
	const tuple_t a_x_b{ tuple_t::cross(a, b) };
	const tuple_t a_x_b_expected{ tuple_t::vector(-1, 2, -1) };
	const tuple_t b_x_a{ tuple_t::cross(b, a) };
	const tuple_t b_x_a_expected{ tuple_t::vector(1, -2, 1) };
	EXPECT_EQ(a_x_b, a_x_b_expected);
	EXPECT_EQ(b_x_a, b_x_a_expected);
}

/*
Scenario: Crossing vector with point or point with vector
  Given v ← vector(1, 2, 3)
	And p ← point(1, 2, 3)
  Then cross(v, p) throws invalid argument exception
  And cross(p, v) throws invalid argument exception
  And cross(p, p) throws invalid argument exception
*/
TEST(tuple, should_throw_when_trying_to_cross_points)
{
	const tuple_t v{ tuple_t::vector(1, 2, 3) };
	const tuple_t p{ tuple_t::point(1, 2, 3) };
	EXPECT_THROW(tuple_t::cross(v, p), std::invalid_argument);
	EXPECT_THROW(tuple_t::cross(p, v), std::invalid_argument);
	EXPECT_THROW(tuple_t::cross(p, p), std::invalid_argument);
}

/*
Scenario: Reflecting a vector approaching at 45°
  Given v ← vector(1, -1, 0)
	And n ← vector(0, 1, 0)
  When r ← v.reflect(n)
  Then r = vector(1, 1, 0)
*/
TEST(tuple, should_reflect_about_a_normal)
{
	const tuple_t v{ tuple_t::vector(1, -1, 0) };
	const tuple_t n{ tuple_t::vector(0, 1, 0) };
	const tuple_t r{ tuple_t::vector(1, 1, 0) };
	EXPECT_EQ(v.reflect(n), r);
}

/*
Scenario: Reflecting a vector off a slanted surface
  Given v ← vector(0, -1, 0)
	And n ← vector(√2/2, √2/2, 0)
  When r ← v.reflect(n)
  Then r = vector(1, 0, 0)
*/
TEST(tuple, should_reflect_about_a_normal_off_a_slanted_surface)
{
	const tuple_t v{ tuple_t::vector(0, -1, 0) };
	const tuple_t n{ tuple_t::vector(std::sqrt(2) / 2, std::sqrt(2) / 2, 0)};
	const tuple_t r{ tuple_t::vector(1, 0, 0) };
	EXPECT_EQ(v.reflect(n), r);
}