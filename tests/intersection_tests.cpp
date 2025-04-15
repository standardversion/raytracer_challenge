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
    const auto s{ Sphere::create() };
    const double t{ 3.5 };
    intersections_t intersections{};
    intersections.add(t, s);
    EXPECT_EQ(intersections[0].time, 3.5);
    EXPECT_EQ(intersections[0].object.get(), s.get());
}

/*
Scenario: Aggregating intersections
  Given s ← sphere()
    And i1 ← intersection(1, s)
    And i2 ← intersection(2, s)
    And i ← intersections()
  When xs ← intersections.add(i1, i2)
  Then xs.count = 2
    And xs[0].t = 1
    And xs[1].t = 2
*/
TEST(intersect, should_be_able_to_aggregate_intersection)
{
    const auto s{ Sphere::create() };
    const double t{ 3.5 };
    const double t2{ 7.0 };
    intersections_t intersections{};
    const intersection_t intersection1{t, s};
    const intersection_t intersection2{t2, s };
    intersections.add(intersection1, intersection2);
    EXPECT_EQ(intersections[0].time, 3.5);
    EXPECT_EQ(intersections[0].object.get(), s.get());
    EXPECT_EQ(intersections[1].time, 7.0);
    EXPECT_EQ(intersections[0].object.get(), s.get());
}

/*
Scenario: The hit, when all intersections have positive t
  Given s ← sphere()
    And i1 ← intersection(1, s)
    And i2 ← intersection(2, s)
    And i ← intersections()
    And xs ← intersections(i2, i1)
  When ir ← intersections.hit()
  Then ir = i1
*/
TEST(intersect, should_return_lowest_positive_intersection)
{
    const auto s{ Sphere::create() };
    const double t{ 1 };
    const double t2{ 2 };
    const intersection_t i{ t, s };
    intersections_t intersections{};
    const intersection_t intersection1{ t, s };
    const intersection_t intersection2{ t2, s };
    intersections.add(intersection2, intersection1);
    const intersection_t ir{ intersections.hit() };
    EXPECT_EQ(i, ir);
}

/*
Scenario: The hit, when some intersections have negative t
  Given s ← sphere()
    And i1 ← intersection(-1, s)
    And i2 ← intersection(1, s)
    And i ← intersections()
    And xs ← intersections(i2, i1)
  When ir ← intersections.hit()
  Then ir = i1
*/
TEST(intersect, should_return_lowest_positive_intersection_even_when_negative_t_exist)
{
    const auto s{ Sphere::create() };
    const double t{ -1 };
    const double t2{ 1 };
    const intersection_t i{ t2, s };
    intersections_t intersections{};
    const intersection_t intersection1{ t, s };
    const intersection_t intersection2{ t2, s };
    intersections.add(intersection2, intersection1);
    const intersection_t ir{ intersections.hit() };
    EXPECT_EQ(i, ir);
}

/*
Scenario: The hit, when all intersections have negative t
  Given s ← sphere()
    And i1 ← intersection(-2, s)
    And i2 ← intersection(-1, s)
    And i ← intersections()
    And xs ← intersections(i2, i1)
  When ir ← intersections.hit()
  Then ir.entries.size() = 0
*/
TEST(intersect, should_return_empty_intersection_even_only_negative_t_exist)
{
    const auto s{ Sphere::create() };
    const double t{ -2 };
    const double t2{ -1 };
    const intersection_t i{ t2, s };
    intersections_t intersections{};
    const intersection_t intersection1{ t, s };
    const intersection_t intersection2{ t2, s };
    intersections.add(intersection2, intersection1);
    const intersection_t ir{ intersections.hit() };
    EXPECT_EQ(ir.object, nullptr);
}

/*
Scenario: The hit is always the lowest nonnegative intersection
  Given s ← sphere()
  And i1 ← intersection(5, s)
  And i2 ← intersection(7, s)
  And i3 ← intersection(-3, s)
  And i4 ← intersection(2, s)
  And xs ← intersections(i1, i2, i3, i4)
When ir ← intersections.hit()
  Then ir = i4
*/
TEST(intersect, should_always_return_lowest_positive_intersection)
{
    const auto s{ Sphere::create() };
    const double t{ 5 };
    const double t2{ 7 };
    const double t3{ -3 };
    const double t4{ 2 };
    const intersection_t i{ t4, s };
    intersections_t intersections{};
    const intersection_t intersection1{ t, s };
    const intersection_t intersection2{ t2, s };
    const intersection_t intersection3{ t3, s };
    const intersection_t intersection4{ t4, s };
    intersections.add(intersection1, intersection2, intersection3, intersection4);
    const intersection_t ir{ intersections.hit() };
    EXPECT_EQ(ir, i);
}