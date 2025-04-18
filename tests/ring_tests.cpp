#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "../colour.h"
#include "../Ring.h"
#include "../tuple.h"
#include "../sphere.h"
#include "../matrix.h"

/*
Scenario: A ring should extend in both x and z
  Given pattern ← ring_pattern(white, black)
  Then pattern.at(point(0, 0, 0) = white
	And pattern.at(point(1, 0, 0) = black
	And pattern.at(point(0, 0, 1) = black
	# 0.708 = just slightly more than √2/2
	And pattern.at(point(0.708, 0, 0.708) = black
*/
TEST(ring, should_have_ring_pattern)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	const Ring ring{ white, black };
	EXPECT_EQ(ring.at(tuple_t::point(0, 0, 0)), white);
	EXPECT_EQ(ring.at(tuple_t::point(1, 0, 0)), black);
	EXPECT_EQ(ring.at(tuple_t::point(0, 0, 1)), black);
	EXPECT_EQ(ring.at(tuple_t::point(0.708, 0, 0.708)), black);
}