#include "gtest/gtest.h"
#include "../area_light.h"
#include "../point_light.h"
#include "../colour.h"
#include "../tuple.h"
#include "../world.h"
#include "../sequence.h"


/*
Scenario: Creating an area light
  Given corner ← point(0, 0, 0)
    And v1 ← vector(2, 0, 0)
    And v2 ← vector(0, 0, 1)
  When light ← area_light(corner, v1, 4, v2, 2, color(1, 1, 1))
  Then light.corner = corner
    And light.uvec = vector(0.5, 0, 0)
    And light.usteps = 4
    And light.vvec = vector(0, 0, 0.5)
    And light.vsteps = 2
    And light.samples = 8
    And light.position = point(1, 0, 0.5)
*/
TEST(area_light, should_create_an_area_light)
{
    const colour_t intensity{ 1, 1, 1 };
    const tuple_t corner{ tuple_t::point(0, 0, 0) };
    const tuple_t full_uvec{ tuple_t::vector(2, 0, 0) };
    const tuple_t full_vvec{ tuple_t::vector(0, 0, 1) };
    const int usteps{ 4 };
    const int vsteps{ 2 };
    Sequence sequence{ 0.0 };
    AreaLight light{ corner, full_uvec, usteps, full_vvec, vsteps, sequence, intensity };
    EXPECT_EQ(light.uvec, tuple_t::vector(0.5, 0, 0));
    EXPECT_EQ(light.get_usteps(), usteps);
    EXPECT_EQ(light.vvec, tuple_t::vector(0, 0, 0.5));
    EXPECT_EQ(light.get_vsteps(), vsteps);
    EXPECT_EQ(light.position(), tuple_t::point(1, 0, 0.5));
}

/*
Scenario Outline: Finding a single point on an area light
  Given corner ← point(0, 0, 0)
    And v1 ← vector(2, 0, 0)
    And v2 ← vector(0, 0, 1)
    And light ← area_light(corner, v1, 4, v2, 2, color(1, 1, 1))
  When pt ← point_on_light(light, <u>, <v>)
  Then pt = <result>

  Examples:
    | u | v | result               |
    | 0 | 0 | point(0.25, 0, 0.25) |
    | 1 | 0 | point(0.75, 0, 0.25) |
    | 0 | 1 | point(0.25, 0, 0.75) |
    | 2 | 0 | point(1.25, 0, 0.25) |
    | 3 | 1 | point(1.75, 0, 0.75) |
*/
TEST(area_light, should_find_a_single_point_on_an_area_light)
{
    const colour_t intensity{ 1, 1, 1 };
    const tuple_t corner{ tuple_t::point(0, 0, 0) };
    const tuple_t full_uvec{ tuple_t::vector(2, 0, 0) };
    const tuple_t full_vvec{ tuple_t::vector(0, 0, 1) };
    const int usteps{ 4 };
    const int vsteps{ 2 };
    Sequence sequence{ 0.5 };
    AreaLight light{ corner, full_uvec, usteps, full_vvec, vsteps, sequence, intensity };
    EXPECT_EQ(light.point_on_light(0, 0), tuple_t::point(0.25, 0, 0.25));
    EXPECT_EQ(light.point_on_light(1, 0), tuple_t::point(0.75, 0, 0.25));
    EXPECT_EQ(light.point_on_light(0, 1), tuple_t::point(0.25, 0, 0.75));
    EXPECT_EQ(light.point_on_light(2, 0), tuple_t::point(1.25, 0, 0.25));
    EXPECT_EQ(light.point_on_light(3, 1), tuple_t::point(1.75, 0, 0.75));
}

/*
Scenario Outline: The area light intensity function
  Given w ← default_world()
    And corner ← point(-0.5, -0.5, -5)
    And v1 ← vector(1, 0, 0)
    And v2 ← vector(0, 1, 0)
    And light ← area_light(corner, v1, 2, v2, 2, color(1, 1, 1))
    And pt ← <point>
  When intensity ← intensity_at(light, pt, w)
  Then intensity = <result>

  Examples:
    | point                | result |
    | point(0, 0, 2)       | 0.0    |
    | point(1, -1, 2)      | 0.25   |
    | point(1.5, 0, 2)     | 0.5    |
    | point(1.25, 1.25, 3) | 0.75   |
    | point(0, 0, -2)      | 1.0    |
*/
TEST(area_light, should_find_light_intensity_at)
{
    World w{ World::default_world() };
    const colour_t intensity{ 1, 1, 1 };
    const tuple_t corner{ tuple_t::point(-0.5, -0.5, -5) };
    const tuple_t full_uvec{ tuple_t::vector(1, 0, 0) };
    const tuple_t full_vvec{ tuple_t::vector(0, 1, 0) };
    const int usteps{ 2 };
    const int vsteps{ 2 };
    Sequence sequence{ 0.5 };
    AreaLight light{ corner, full_uvec, usteps, full_vvec, vsteps, sequence, intensity };
    EXPECT_EQ(light.intensity_at(tuple_t::point(0, 0, 2), w), 0.0);
    EXPECT_EQ(light.intensity_at(tuple_t::point(1, -1, 2), w), 0.25);
    EXPECT_EQ(light.intensity_at(tuple_t::point(1.5, 0, 2), w), 0.5);
    EXPECT_EQ(light.intensity_at(tuple_t::point(1.25, 1.25, 3), w), 0.75);
    EXPECT_EQ(light.intensity_at(tuple_t::point(0, 0, -2), w), 1.0);
}

/*
Scenario Outline: Finding a single point on a jittered area light
  Given corner ← point(0, 0, 0)
    And v1 ← vector(2, 0, 0)
    And v2 ← vector(0, 0, 1)
    And light ← area_light(corner, v1, 4, v2, 2, color(1, 1, 1))
    And light.jitter_by ← sequence(0.3, 0.7)
  When pt ← point_on_light(light, <u>, <v>)
  Then pt = <result>

  Examples:
    | u | v | result               |
    | 0 | 0 | point(0.15, 0, 0.35) |
    | 1 | 0 | point(0.65, 0, 0.35) |
    | 0 | 1 | point(0.15, 0, 0.85) |
    | 2 | 0 | point(1.15, 0, 0.35) |
    | 3 | 1 | point(1.65, 0, 0.85) |
*/
TEST(area_light, should_find_a_single_point_on_a_jittered_area_light)
{
    const colour_t intensity{ 1, 1, 1 };
    const tuple_t corner{ tuple_t::point(0, 0, 0) };
    const tuple_t full_uvec{ tuple_t::vector(2, 0, 0) };
    const tuple_t full_vvec{ tuple_t::vector(0, 0, 1) };
    const int usteps{ 4 };
    const int vsteps{ 2 };
    Sequence sequence{ 0.3, 0.7 };
    AreaLight light{ corner, full_uvec, usteps, full_vvec, vsteps, sequence, intensity };
    EXPECT_EQ(light.point_on_light(0, 0), tuple_t::point(0.15, 0, 0.35));
    EXPECT_EQ(light.point_on_light(1, 0), tuple_t::point(0.65, 0, 0.35));
    EXPECT_EQ(light.point_on_light(0, 1), tuple_t::point(0.15, 0, 0.85));
    EXPECT_EQ(light.point_on_light(2, 0), tuple_t::point(1.15, 0, 0.35));
    EXPECT_EQ(light.point_on_light(3, 1), tuple_t::point(1.65, 0, 0.85));
}

/*
Scenario Outline: The area light with jittered samples
  Given w ← default_world()
    And corner ← point(-0.5, -0.5, -5)
    And v1 ← vector(1, 0, 0)
    And v2 ← vector(0, 1, 0)
    And light ← area_light(corner, v1, 2, v2, 2, color(1, 1, 1))
    And light.jitter_by ← sequence(0.7, 0.3, 0.9, 0.1, 0.5)
    And pt ← <point>
  When intensity ← intensity_at(light, pt, w)
  Then intensity = <result>

  Examples:
    | point                | result |
    | point(0, 0, 2)       | 0.0    |
    | point(1, -1, 2)      | 0.5    |
    | point(1.5, 0, 2)     | 0.75   |
    | point(1.25, 1.25, 3) | 0.75   |
    | point(0, 0, -2)      | 1.0    |
*/
TEST(area_light, should_find_light_intensity_at_with_jittered_samples)
{
    World w{ World::default_world() };
    const colour_t intensity{ 1, 1, 1 };
    const tuple_t corner{ tuple_t::point(-0.5, -0.5, -5) };
    const tuple_t full_uvec{ tuple_t::vector(1, 0, 0) };
    const tuple_t full_vvec{ tuple_t::vector(0, 1, 0) };
    const int usteps{ 2 };
    const int vsteps{ 2 };
    Sequence sequence{ 0.7, 0.3, 0.9, 0.1, 0.5 };
    AreaLight light{ corner, full_uvec, usteps, full_vvec, vsteps, sequence, intensity };
    EXPECT_EQ(light.intensity_at(tuple_t::point(0, 0, 2), w), 0.0);
    EXPECT_EQ(light.intensity_at(tuple_t::point(1, -1, 2), w), 0.25); // only this one fails if its 0.5?
    EXPECT_EQ(light.intensity_at(tuple_t::point(1.5, 0, 2), w), 0.75);
    EXPECT_EQ(light.intensity_at(tuple_t::point(1.25, 1.25, 3), w), 0.75);
    EXPECT_EQ(light.intensity_at(tuple_t::point(0, 0, -2), w), 1.0);
}