#include "gtest/gtest.h"
#include "../light.h"
#include "../colour.h"
#include "../tuple.h"

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