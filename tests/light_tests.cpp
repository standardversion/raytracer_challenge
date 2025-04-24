#include "gtest/gtest.h"
#include "../light.h"
#include "../colour.h"
#include "../tuple.h"
#include "../world.h"

/*
Scenario: A point light has a position and intensity
  Given intensity ← color(1, 1, 1)
    And position ← point(0, 0, 0)
  When light ← point_light(position, intensity)
  Then light.position = position
    And light.intensity = intensity
*/
TEST(light, sholud_create_light_with_intensity_and_position)
{
    const colour_t intensity{ 1, 1, 1 };
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    Light point_light{ intensity };
    EXPECT_EQ(point_light.position(), position);
    EXPECT_EQ(point_light.intensity, intensity);
}

/*
Scenario: A point light evaluates the light intensity at a given point
  Given w ← default_world()
    And light ← w.light
    And pt ← <point>
  When intensity ← intensity_at(light, pt, w)
  Then intensity = <result>

  Examples:
    | point                | result |
    | point(0, 1.0001, 0)  | 1.0    |
    | point(-1.0001, 0, 0) | 1.0    |
    | point(0, 0, -1.0001) | 1.0    |
    | point(0, 0, 1.0001)  | 0.0    |
    | point(1.0001, 0, 0)  | 0.0    |
    | point(0, -1.0001, 0) | 0.0    |
    | point(0, 0, 0)       | 0.0    |
*/
TEST(light, sholud_evaluate_light_intensity_at_a_given_point_for_point_lights)
{
    const World w{ World::default_world() };
    auto l{ w.lights[0].lock() };
    EXPECT_EQ(l->intensity_at(tuple_t::point(0, 1.0001, 0), w), 1.0);
    EXPECT_EQ(l->intensity_at(tuple_t::point(-1.0001, 0, 0), w), 1.0);
    EXPECT_EQ(l->intensity_at(tuple_t::point(0, 0, -1.0001), w), 1.0);
    EXPECT_EQ(l->intensity_at(tuple_t::point(0, 0, 1.0001), w), 0.0);
    EXPECT_EQ(l->intensity_at(tuple_t::point(1.0001, 0, 0), w), 0.0);
    EXPECT_EQ(l->intensity_at(tuple_t::point(0, -1.0001, 0), w), 0.0);
    EXPECT_EQ(l->intensity_at(tuple_t::point(0, 0, 0), w), 0.0);
}