#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "../colour.h"
#include "../checker.h"
#include "../tuple.h"
#include "../sphere.h"
#include "../matrix.h"

/*
Scenario: Checkers should repeat in x
  Given pattern ← checkers_pattern(white, black)
  Then pattern.at(point(0, 0, 0) = white
	And pattern.at(point(0.99, 0, 0) = white
	And pattern.at(point(1.01, 0, 0) = black
*/
TEST(checker, should_repeat_pattern_in_x)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	const Checker checker{ white, black };
	EXPECT_EQ(checker.at(tuple_t::point(0, 0, 0)), white);
	EXPECT_EQ(checker.at(tuple_t::point(0.99, 0, 0)), white);
	EXPECT_EQ(checker.at(tuple_t::point(1.01, 0, 0)), black);
}

/*
Scenario: Checkers should repeat in y
  Given pattern ← checkers_pattern(white, black)
  Then pattern.at(point(0, 0, 0) = white
	And pattern.at(point(0, 0.99, 0) = white
	And pattern.at(point(0, 1.01, 0) = black
*/
TEST(checker, should_repeat_pattern_in_y)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	const Checker checker{ white, black };
	EXPECT_EQ(checker.at(tuple_t::point(0, 0, 0)), white);
	EXPECT_EQ(checker.at(tuple_t::point(0, 0.99, 0)), white);
	EXPECT_EQ(checker.at(tuple_t::point(0, 1.01, 0)), black);
}

/*
Scenario: Checkers should repeat in z
  Given pattern ← checkers_pattern(white, black)
  Then pattern.at(point(0, 0, 0) = white
	And pattern.at(point(0, 0, 0.99) = white
	And pattern.at(point(0, 0, 1.01) = black
*/
TEST(checker, should_repeat_pattern_in_z)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	const Checker checker{ white, black };
	EXPECT_EQ(checker.at(tuple_t::point(0, 0, 0)), white);
	EXPECT_EQ(checker.at(tuple_t::point(0, 0, 0.99)), white);
	EXPECT_EQ(checker.at(tuple_t::point(0, 0, 1.01)), black);
}

/*
Scenario Outline: Checker pattern in 2D
  Given checkers ← uv_checkers(2, 2, black, white)
  When color ← uv_pattern_at(checkers, <u>, <v>)
  Then color = <expected>

  Examples:
	| u   | v   | expected |
	| 0.0 | 0.0 | black    |
	| 0.5 | 0.0 | white    |
	| 0.0 | 0.5 | white    |
	| 0.5 | 0.5 | black    |
	| 1.0 | 1.0 | black    |
*/
TEST(checker, should_get_colour_at_uv_pattern)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	const Checker checker{ 2, 2, black, white };
	EXPECT_EQ(checker.at(0.0, 0.0), black);
	EXPECT_EQ(checker.at(0.5, 0.0), white);
	EXPECT_EQ(checker.at(0.0, 0.5), white);
	EXPECT_EQ(checker.at(0.5, 0.5), black);
	EXPECT_EQ(checker.at(1.0, 1.0), black);
}