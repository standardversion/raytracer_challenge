#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "../colour.h"
#include "../pattern.h"
#include "../tuple.h"
#include "../sphere.h"
#include "../matrix.h"

/*
Scenario: Creating a stripe pattern
  Given pattern ← stripe_pattern(white, black)
  Then pattern.a = white
	And pattern.b = black
*/
TEST(pattern, should_have_a_b_colours_for_stripe_pattern)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	const pattern_t stripe{ white, black };
	EXPECT_EQ(stripe.a, white);
	EXPECT_EQ(stripe.b, black);
}

/*
Scenario: A stripe pattern is constant in y
  Given pattern ← stripe_pattern(white, black)
  Then pattern.at(point(0, 0, 0)) = white
	And pattern.at(point(0, 1, 0)) = white
	And pattern.at(point(0, 2, 0)) = white
*/
TEST(pattern, should_be_a_constant_colour_in_y)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	const pattern_t stripe{ white, black };
	EXPECT_EQ(stripe.at(tuple_t::point(0, 0, 0)), white);
	EXPECT_EQ(stripe.at(tuple_t::point(0, 1, 0)), white);
	EXPECT_EQ(stripe.at(tuple_t::point(0, 2, 0)), white);
}

/*
Scenario: A stripe pattern is constant in z
  Given pattern ← stripe_pattern(white, black)
  Then pattern.at(point(0, 0, 0)) = white
	And pattern.at(point(0, 0, 1)) = white
	And pattern.at(point(0, 0, 2)) = white
*/
TEST(pattern, should_be_a_constant_colour_in_z)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	const pattern_t stripe{ white, black };
	EXPECT_EQ(stripe.at(tuple_t::point(0, 0, 0)), white);
	EXPECT_EQ(stripe.at(tuple_t::point(0, 0, 1)), white);
	EXPECT_EQ(stripe.at(tuple_t::point(0, 0, 2)), white);
}

/*
Scenario: A stripe pattern alternates in x
  Given pattern ← stripe_pattern(white, black)
  Then pattern.at(point(0, 0, 0)) = white
	And pattern.at(point(0.9, 0, 0)) = white
	And pattern.at(point(1, 0, 0)) = black
	And pattern.at(point(-0.1, 0, 0)) = black
	And pattern.at(point(-1, 0, 0)) = black
	And pattern.at(point(-1.1, 0, 0)) = white
*/
TEST(pattern, should_alternate_pattern_in_x)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	const pattern_t stripe{ white, black };
	EXPECT_EQ(stripe.at(tuple_t::point(0, 0, 0)), white);
	EXPECT_EQ(stripe.at(tuple_t::point(0.9, 0, 0)), white);
	EXPECT_EQ(stripe.at(tuple_t::point(1, 0, 0)), black);
	EXPECT_EQ(stripe.at(tuple_t::point(-0.1, 0, 0)), black);
	EXPECT_EQ(stripe.at(tuple_t::point(-1, 0, 0)), black);
	EXPECT_EQ(stripe.at(tuple_t::point(-1.1, 0, 0)), white);
}

/*
Scenario: Stripes with an object transformation
  Given object ← sphere()
	And object.transform = scaling(2, 2, 2)
	And pattern ← stripe_pattern(white, black)
  When c ← pattern.at_object(object, point(1.5, 0, 0))
  Then c = white
*/
TEST(pattern, should_work_with_object_transforms)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	const pattern_t stripe{ white, black };
	auto s{ Sphere::create() };
	s->transform = matrix_t::scaling(2, 2, 2);
	EXPECT_EQ(stripe.at_object(dynamic_cast<Geometry*>(s.get()), tuple_t::point(1.5, 0, 0)), white);
}

/*
Scenario: Stripes with a pattern transformation
  Given object ← sphere()
	And pattern ← stripe_pattern(white, black)
	And pattern.transform = scaling(2, 2, 2)
  When c ← pattern.at_object(object, point(1.5, 0, 0))
  Then c = white
*/
TEST(pattern, should_work_with_pattern_transforms)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	pattern_t stripe{ white, black };
	stripe.transform = matrix_t::scaling(2, 2, 2);
	auto s{ Sphere::create() };
	s->transform = matrix_t::scaling(2, 2, 2);
	EXPECT_EQ(stripe.at_object(dynamic_cast<Geometry*>(s.get()), tuple_t::point(1.5, 0, 0)), white);
}

/*
Scenario: Stripes with both an object and a pattern transformation
  Given object ← sphere()
	And object.transform =scaling(2, 2, 2)
	And pattern ← stripe_pattern(white, black)
	And pattern.transform = translation(0.5, 0, 0)
  When c ← pattern.at_object(object, point(2.5, 0, 0))
  Then c = white
*/
TEST(pattern, should_work_with_pattern_and_object_transforms)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	pattern_t stripe{ white, black };
	stripe.transform = matrix_t::translation(0.5, 0, 0);
	auto s{ Sphere::create() };
	s->transform = matrix_t::scaling(2, 2, 2);
	s->transform = matrix_t::scaling(2, 2, 2);
	EXPECT_EQ(stripe.at_object(dynamic_cast<Geometry*>(s.get()), tuple_t::point(2.5, 0, 0)), white);
}