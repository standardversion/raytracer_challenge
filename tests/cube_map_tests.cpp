#include "gtest/gtest.h"
#include "../cube_map.h"
#include "../colour.h"
#include "../align_check.h"
#include "../tuple.h"

/*
Scenario: Finding the colors on a mapped cube
  When red ← color(1, 0, 0)
    And yellow ← color(1, 1, 0)
    And brown ← color(1, 0.5, 0)
    And green ← color(0, 1, 0)
    And cyan ← color(0, 1, 1)
    And blue ← color(0, 0, 1)
    And purple ← color(1, 0, 1)
    And white ← color(1, 1, 1)
    And left ← uv_align_check(yellow, cyan, red, blue, brown)
    And front ← uv_align_check(cyan, red, yellow, brown, green)
    And right ← uv_align_check(red, yellow, purple, green, white)
    And back ← uv_align_check(green, purple, cyan, white, blue)
    And up ← uv_align_check(brown, cyan, purple, red, yellow)
    And down ← uv_align_check(purple, brown, green, blue, white)
    And pattern ← cube_map(left, front, right, back, up, down)
  Then pattern_at(pattern, <point>) = <color>

  Examples:
    |   | point                 | color  |
    | L | point(-1, 0, 0)       | yellow |
    |   | point(-1, 0.9, -0.9)  | cyan   |
    |   | point(-1, 0.9, 0.9)   | red    |
    |   | point(-1, -0.9, -0.9) | blue   |
    |   | point(-1, -0.9, 0.9)  | brown  |
    | F | point(0, 0, 1)        | cyan   |
    |   | point(-0.9, 0.9, 1)   | red    |
    |   | point(0.9, 0.9, 1)    | yellow |
    |   | point(-0.9, -0.9, 1)  | brown  |
    |   | point(0.9, -0.9, 1)   | green  |
    | R | point(1, 0, 0)        | red    |
    |   | point(1, 0.9, 0.9)    | yellow |
    |   | point(1, 0.9, -0.9)   | purple |
    |   | point(1, -0.9, 0.9)   | green  |
    |   | point(1, -0.9, -0.9)  | white  |
    | B | point(0, 0, -1)       | green  |
    |   | point(0.9, 0.9, -1)   | purple |
    |   | point(-0.9, 0.9, -1)  | cyan   |
    |   | point(0.9, -0.9, -1)  | white  |
    |   | point(-0.9, -0.9, -1) | blue   |
    | U | point(0, 1, 0)        | brown  |
    |   | point(-0.9, 1, -0.9)  | cyan   |
    |   | point(0.9, 1, -0.9)   | purple |
    |   | point(-0.9, 1, 0.9)   | red    |
    |   | point(0.9, 1, 0.9)    | yellow |
    | D | point(0, -1, 0)       | purple |
    |   | point(-0.9, -1, 0.9)  | brown  |
    |   | point(0.9, -1, 0.9)   | green  |
    |   | point(-0.9, -1, -0.9) | blue   |
    |   | point(0.9, -1, -0.9)  | white  |
*/
TEST(cube_map, should_find_colour_on_a_mapped_cube)
{
    const colour_t red{ 1, 0, 0 };
    const colour_t yellow{ 1, 1, 0 };
    const colour_t brown{ 1, 0.5, 0 };
    const colour_t green{ 0, 1, 0 };
    const colour_t cyan{ 0, 1, 1 };
    const colour_t blue{ 0, 0, 1 };
    const colour_t purple{ 1, 0, 1 };
    const colour_t white{ 1, 1, 1 };
    const AlignCheck left{ yellow, cyan, red, blue, brown };
    const AlignCheck front{ cyan, red, yellow, brown, green };
    const AlignCheck right{ red, yellow, purple, green, white };
    const AlignCheck back{ green, purple, cyan, white, blue };
    const AlignCheck up{ brown, cyan, purple, red, yellow };
    const AlignCheck down{ purple, brown, green, blue, white };
    const CubeMap cm{ 
        std::make_shared<AlignCheck>(left),
        std::make_shared<AlignCheck>(right),
        std::make_shared<AlignCheck>(front),
        std::make_shared<AlignCheck>(back),
        std::make_shared<AlignCheck>(up),
        std::make_shared<AlignCheck>(down)
    };
    //L
    EXPECT_EQ(cm.at(tuple_t::point(-1, 0, 1)), yellow);
    EXPECT_EQ(cm.at(tuple_t::point(-1, 0.9, -0.9)), cyan);
    EXPECT_EQ(cm.at(tuple_t::point(-1, 0.9, 0.9)), red);
    EXPECT_EQ(cm.at(tuple_t::point(-1, -0.9, -0.9)), blue);
    EXPECT_EQ(cm.at(tuple_t::point(-1, -0.9, 0.9)), brown);
    //F
    EXPECT_EQ(cm.at(tuple_t::point(0, 0, 1)), cyan);
    EXPECT_EQ(cm.at(tuple_t::point(-0.9, 0.9, 1)), red);
    EXPECT_EQ(cm.at(tuple_t::point(0.9, 0.9, 1)), yellow);
    EXPECT_EQ(cm.at(tuple_t::point(-0.9, -0.9, 1)), brown);
    EXPECT_EQ(cm.at(tuple_t::point(0.9, -0.9, 1)), green);
    //R
    EXPECT_EQ(cm.at(tuple_t::point(1, 0, 0)), red);
    EXPECT_EQ(cm.at(tuple_t::point(1, 0.9, 0.9)), yellow);
    EXPECT_EQ(cm.at(tuple_t::point(1, 0.9, -0.9)), purple);
    EXPECT_EQ(cm.at(tuple_t::point(1, -0.9, 0.9)), green);
    EXPECT_EQ(cm.at(tuple_t::point(1, -0.9, -0.9)), white);
    //B
    EXPECT_EQ(cm.at(tuple_t::point(0, 0, -1)), green);
    EXPECT_EQ(cm.at(tuple_t::point(0.9, 0.9, -1)), purple);
    EXPECT_EQ(cm.at(tuple_t::point(-0.9, 0.9, -1)), cyan);
    EXPECT_EQ(cm.at(tuple_t::point(0.9, -0.9, -1)), white);
    EXPECT_EQ(cm.at(tuple_t::point(-0.9, -0.9, -1)), blue);
    //U
    EXPECT_EQ(cm.at(tuple_t::point(0, 1, 0)), brown);
    EXPECT_EQ(cm.at(tuple_t::point(-0.9, 1, -0.9)), cyan);
    EXPECT_EQ(cm.at(tuple_t::point(0.9, 1, -0.9)), purple);
    EXPECT_EQ(cm.at(tuple_t::point(-0.9, 1, 0.9)), red);
    EXPECT_EQ(cm.at(tuple_t::point(0.9, 1, 0.9)), yellow);
    //D
    EXPECT_EQ(cm.at(tuple_t::point(0, -1, 0)), purple);
    EXPECT_EQ(cm.at(tuple_t::point(-0.9, -1, 0.9)), brown);
    EXPECT_EQ(cm.at(tuple_t::point(0.9, -1, 0.9)), green);
    EXPECT_EQ(cm.at(tuple_t::point(-0.9, -1, -0.9)), blue);
    EXPECT_EQ(cm.at(tuple_t::point(0.9, -1, -0.9)), white);
}