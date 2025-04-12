#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "../sphere.h"
#include "../intersection.h"

/*
Scenario: An intersection encapsulates t and object
  Given s ← sphere()
    And i ← intersections()
  When i.add(3.5, s)
  Then i[0].time = 3.5
    And i[0].object = s
*/
TEST(intersect, should_have_time_and_object_attrs)
{
    const sphere_t s{};
    const double t{ 3.5 };
    intersections_t intersections{};
    intersections.add(t, &s);
    EXPECT_EQ(intersections[0].time, 3.5);
    EXPECT_EQ(intersections[0].object, &s);
}
