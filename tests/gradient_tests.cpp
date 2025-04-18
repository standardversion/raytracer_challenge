#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "../colour.h"
#include "../gradient.h"
#include "../tuple.h"
#include "../sphere.h"
#include "../matrix.h"

/*
Scenario: A gradient linearly interpolates between colors
  Given pattern ← gradient_pattern(white, black)
  Then pattern.at(point(0, 0, 0) = white
	And pattern.at(point(0.25, 0, 0) = color(0.75, 0.75, 0.75)
	And pattern.at(point(0.5, 0, 0) = color(0.5, 0.5, 0.5)
	And pattern.at(point(0.75, 0, 0) = color(0.25, 0.25, 0.25)
*/
TEST(gradient, should_have_gradient_pattern)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	const Gradient gradient{ white, black };
	const colour_t r{ 0.75, 0.75, 0.75 };
	const colour_t r2{ 0.5, 0.5, 0.5 };
	const colour_t r3{ 0.25, 0.25, 0.25 };
	EXPECT_EQ(gradient.at(tuple_t::point(0, 0, 0)), white);
	EXPECT_EQ(gradient.at(tuple_t::point(0.25, 0, 0)), r);
	EXPECT_EQ(gradient.at(tuple_t::point(0.5, 0, 0)), r2);
	EXPECT_EQ(gradient.at(tuple_t::point(0.75, 0, 0)), r3);
}