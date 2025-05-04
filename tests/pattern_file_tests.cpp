#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "../pattern_file.h"

/*
Scenario: Checker pattern in 2D
  Given ppm ← a file containing:
    """
    P3
    10 10
    10
    0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9
    1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0
    2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1
    3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2
    4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3
    5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4
    6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5
    7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6
    8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7
    9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8
    """
    And canvas ← canvas_from_ppm(ppm)
    And pattern ← uv_image(canvas)
  When color ← uv_pattern_at(pattern, <u>, <v>)
  Then color = <expected>

  Examples:
    | u   | v   | expected             |
    | 0   | 0   | color(0.9, 0.9, 0.9) |
    | 0.3 | 0   | color(0.2, 0.2, 0.2) |
    | 0.6 | 0.3 | color(0.1, 0.1, 0.1) |
    | 1   | 1   | color(0.9, 0.9, 0.9) |
*/
TEST(pattern_file, should_read_colour_values_from_file)
{
    const PatternFile pt{ "..\\..\\tests\\in6.ppm" };
    EXPECT_EQ(pt.at(0, 0), colour_t(0.9, 0.9, 0.9));
    EXPECT_EQ(pt.at(0.3, 0), colour_t(0.2, 0.2, 0.2));
    EXPECT_EQ(pt.at(0.6, 0.3), colour_t(0.1, 0.1, 0.1));
    EXPECT_EQ(pt.at(1, 1), colour_t(0.9, 0.9, 0.9));
}