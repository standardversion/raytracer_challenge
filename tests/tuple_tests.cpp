#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../tuple.h"

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

TEST(tuple, should_create_a_tuple_using_point_static_func)
{
	const tuple_t p_point{ tuple_t::point(4, -4, 3) };
	const tuple_t p_tuple{ 4, -4, 3, 1.0 };
	EXPECT_EQ(p_point.x, p_tuple.x);
	EXPECT_EQ(p_point.y, p_tuple.y);
	EXPECT_EQ(p_point.z, p_tuple.z);
	EXPECT_EQ(p_point.w, p_tuple.w);
}

TEST(tuple, should_create_a_tuple_using_vector_static_func)
{
	const tuple_t v_vector{ tuple_t::vector(4, -4, 3) };
	const tuple_t v_tuple{ 4, -4, 3, 0.0 };
	EXPECT_EQ(v_vector.x, v_tuple.x);
	EXPECT_EQ(v_vector.y, v_tuple.y);
	EXPECT_EQ(v_vector.z, v_tuple.z);
	EXPECT_EQ(v_vector.w, v_tuple.w);
}

TEST(tuple, should_implement_comparison_operator)
{
	const tuple_t a{ 4.3, -4.2, 3.1, 0.0 };
	const tuple_t b{ 4.3, -4.2, 3.1, 1.0 };
	const tuple_t c{ 4.3, -4.2, 3.1, 0.0 };
	EXPECT_FALSE(a == b);
	EXPECT_TRUE(a == c);
}

TEST(tuple, should_implement_addition_operator)
{
	const tuple_t a{ 3, -2, 5, 1 };
	const tuple_t b{ -2, 3, 1, 0 };
	const tuple_t c{ a + b };
	const tuple_t expected{ 1, 1, 6, 1 };
	EXPECT_EQ(c, expected);
}

TEST(tuple, should_throw_invalid_arg_when_adding_two_points)
{
	const tuple_t a{ 3, -2, 5, 1 };
	const tuple_t b{ -2, 3, 1, 1 };
	EXPECT_THROW(a + b, std::invalid_argument);
}

TEST(tuple, should_implement_subtraction_operator_two_points_gives_a_vector)
{
	const tuple_t p1{ tuple_t::point(3, 2, 1) };
	const tuple_t p2{ tuple_t::point(5, 6, 7) };
	const tuple_t v{ tuple_t::vector(-2, -4, -6) };
	EXPECT_EQ(p1 - p2, v);
	EXPECT_TRUE(v.is_vector());
}

TEST(tuple, should_result_is_a_point_when_vector_is_subtracted_from_a_point)
{
	const tuple_t p{ tuple_t::point(3, 2, 1) };
	const tuple_t v{ tuple_t::vector(5, 6, 7) };
	const tuple_t r{ tuple_t::point(-2, -4, -6) };
	EXPECT_EQ(p - v, r);
	EXPECT_TRUE(r.is_point());
}

TEST(tuple, should_result_is_a_vector_when_two_vectors_are_subtracted)
{
	const tuple_t v1{ tuple_t::vector(3, 2, 1) };
	const tuple_t v2{ tuple_t::vector(5, 6, 7) };
	const tuple_t r{ tuple_t::vector(-2, -4, -6) };
	EXPECT_EQ(v1 - v2, r);
	EXPECT_TRUE(r.is_vector());
}