#include "gtest/gtest.h"
#include "../light.h"
#include "../colour.h"
#include "../tuple.h"
#include "../world.h"

// Test helper class since SceneObject consturctor is protected
class TestLight : public Light
{
public:
    TestLight(colour_t intensity)
        : Light{ intensity }
    {
    }
    bool operator==(const Light& l) const
    {
        return false;
    }

    double intensity_at(const tuple_t& point, const World& w) const
    {
        return 0.0;
    }
};

/*
Scenario: A light has a position and intensity
  Given intensity ← color(1, 1, 1)
    And position ← point(0, 0, 0)
  When light ← light(position, intensity)
  Then light.position = position
    And light.intensity = intensity
*/
TEST(light, sholud_create_light_with_intensity_and_position)
{
    const colour_t intensity{ 1, 1, 1 };
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    TestLight light{ intensity };
    EXPECT_EQ(light.position(), position);
    EXPECT_EQ(light.intensity, intensity);
}