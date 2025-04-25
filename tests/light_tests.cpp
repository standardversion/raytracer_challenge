#include "gtest/gtest.h"
#include "../light.h"
#include "../colour.h"
#include "../tuple.h"
#include "../world.h"

// Test helper class since Light consturctor is abstract
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

    double intensity_at(const tuple_t& point, const World& w)
    {
        return 0.0;
    }
    tuple_t point_on_light(const double u, const double v)
    {
        return tuple_t::point(0, 0, 0);
    }
};

/*
Scenario: A light has intensity, usteps, vsteps
  Given intensity ← color(1, 1, 1)
    And position ← point(0, 0, 0)
  When light ← light(position, intensity)
  Then light.get_usteps() = 1
    And light.get_vsteps() = 1
    And light.intensity = intensity
*/
TEST(light, should_create_light_with_intensity_and_position)
{
    const colour_t intensity{ 1, 1, 1 };
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    TestLight light{ intensity };
    EXPECT_EQ(light.intensity, intensity);
    EXPECT_EQ(light.get_usteps(), 1);
    EXPECT_EQ(light.get_vsteps(), 1);
}