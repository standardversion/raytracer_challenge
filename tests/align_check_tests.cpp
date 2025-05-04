#include "gtest/gtest.h"
#include "../colour.h"
#include "../align_check.h"
#include "../tuple.h"
#include "../sphere.h"
#include "../matrix.h"


/*
Scenario: Layout of the "align check" pattern
  Given main ← color(1, 1, 1)
    And ul ← color(1, 0, 0)
    And ur ← color(1, 1, 0)
    And bl ← color(0, 1, 0)
    And br ← color(0, 1, 1)
    And pattern ← uv_align_check(main, ul, ur, bl, br)
  When c ← uv_pattern_at(pattern, <u>, <v>)
  Then c = <expected>

  Examples:
    | u    | v    | expected |
    | 0.5  | 0.5  | main     |
    | 0.1  | 0.9  | ul       |
    | 0.9  | 0.9  | ur       |
    | 0.1  | 0.1  | bl       |
    | 0.9  | 0.1  | br       |
*/
TEST(align_check, should_return_correct_colour)
{
    const colour_t main{ 1, 1, 1 };
	const colour_t ul{ 1, 0, 0 };
    const colour_t ur{ 1, 1, 0 };
    const colour_t bl{ 0, 1, 0 };
    const colour_t br{ 0, 1, 1 };
    const AlignCheck ac{ main, ul, ur, bl, br };
	EXPECT_EQ(ac.at(0.5, 0.5), main);
    EXPECT_EQ(ac.at(0.1, 0.9), ul);
    EXPECT_EQ(ac.at(0.9, 0.9), ur);
    EXPECT_EQ(ac.at(0.1, 0.1), bl);
    EXPECT_EQ(ac.at(0.9, 0.1), br);
}
