#include <vector>
#include "gtest/gtest.h"
#include "../cylinder.h"
#include "../ray.h"
#include "../intersection.h"
#include "../bounding_box.h"

/*
Scenario Outline: A ray misses a cylinder
  Given cyl ← cylinder()
    And direction ← normalize(<direction>)
    And r ← ray(<origin>, direction)
  When xs ← local_intersect(cyl, r)
  Then xs.count = 0

  Examples:
    | origin          | direction       |
    | point(1, 0, 0)  | vector(0, 1, 0) |
    | point(0, 0, 0)  | vector(0, 1, 0) |
    | point(0, 0, -5) | vector(1, 1, 1) |
*/
TEST(cylinder, should_miss_the_cylinder)
{
    const auto c{ Cylinder::create() };
    std::vector<tuple_t> points{
        tuple_t::point(1, 0, 0),
        tuple_t::point(0, 0, 0),
        tuple_t::point(0, 0, -5)
    };
    std::vector<tuple_t> directions{
        tuple_t::vector(0, 1, 0),
        tuple_t::vector(0, 1, 0),
        tuple_t::vector(1, 1, 1)
    };
    std::size_t index{ 0 };
    intersections_t i{};
    for (const auto& p : points)
    {
        tuple_t direction{ directions[index] };
        direction.normalize();
        const ray_t r{ p, direction };
        c->intersect(r, i);
        EXPECT_EQ(i.entries.size(), 0);
        i.entries.clear();
        index++;
    }
}

/*
Scenario Outline: A ray strikes a cylinder
  Given cyl ← cylinder()
    And direction ← normalize(<direction>)
    And r ← ray(<origin>, direction)
  When xs ← local_intersect(cyl, r)
  Then xs.count = 2
    And xs[0].t = <t0>
    And xs[1].t = <t1>

  Examples:
    | origin            | direction         | t0      | t1      |
    | point(1, 0, -5)   | vector(0, 0, 1)   | 5       | 5       |
    | point(0, 0, -5)   | vector(0, 0, 1)   | 4       | 6       |
    | point(0.5, 0, -5) | vector(0.1, 1, 1) | 6.80798 | 7.08872 |
*/
TEST(cylinder, should_intersect_with_the_cylinder)
{
    const auto c{ Cylinder::create() };
    std::vector<tuple_t> points{
        tuple_t::point(1, 0, -5),
        tuple_t::point(0, 0, -5),
        tuple_t::point(0.5, 0, -5)
    };
    std::vector<tuple_t> directions{
        tuple_t::vector(0, 0, 1),
        tuple_t::vector(0, 0, 1),
        tuple_t::vector(0.1, 1, 1)
    };
    std::size_t index{ 0 };
    intersections_t i{};
    for (const auto& p : points)
    {
        tuple_t direction{ directions[index] };
        direction.normalize();
        const ray_t r{ p, direction };
        c->intersect(r, i);
        EXPECT_EQ(i.entries.size(), 2);
        if (index == 0)
        {
            EXPECT_EQ(i[0].time, 5);
            EXPECT_EQ(i[1].time, 5);
        }
        else if (index == 1)
        {
            EXPECT_EQ(i[0].time, 4);
            EXPECT_EQ(i[1].time, 6);
        }
        else
        {
            EXPECT_NEAR(i[0].time, 6.80798, 0.001);
            EXPECT_NEAR(i[1].time, 7.08872, 0.001);
        }
        i.entries.clear();
        index++;
    }
}

/*
Scenario Outline: Normal vector on a cylinder
  Given cyl ← cylinder()
  When n ← local_normal_at(cyl, <point>)
  Then n = <normal>

  Examples:
    | point           | normal           |
    | point(1, 0, 0)  | vector(1, 0, 0)  |
    | point(0, 5, -1) | vector(0, 0, -1) |
    | point(0, -2, 1) | vector(0, 0, 1)  |
    | point(-1, 1, 0) | vector(-1, 0, 0) |
*/
TEST(cylinder, should_calculate_normal_at_point)
{
    const auto c{ Cylinder::create() };
    std::vector<tuple_t> points{
        tuple_t::point(1, 0, 0),
        tuple_t::point(0, 5, -1),
        tuple_t::point(0, -2, 1),
        tuple_t::point(-1, 1, 0)
    };
    std::vector<tuple_t> normals{
        tuple_t::vector(1, 0, 0),
        tuple_t::vector(0, 0, -1),
        tuple_t::vector(0, 0, 1),
        tuple_t::vector(-1, 0, 0)
    };
    std::size_t index{ 0 };
    for (const auto& p : points)
    {
        tuple_t direction{ normals[index] };
        direction.normalize();
        const tuple_t n{ c->normal_at(p) };
        EXPECT_EQ(n, normals[index]);
        index++;
    }
}

/*
Scenario: The default minimum and maximum for a cylinder
  Given cyl ← cylinder()
  Then cyl.minimum = -infinity
    And cyl.maximum = infinity
*/
TEST(cylinder, should_have_default_min_max_values)
{
    const auto c{ Cylinder::create() };
    EXPECT_EQ(c->minimum, -INFINITY);
    EXPECT_EQ(c->maximum, INFINITY);
}

/*
Scenario Outline: Intersecting a constrained cylinder
  Given cyl ← cylinder()
    And cyl.minimum ← 1
    And cyl.maximum ← 2
    And direction ← normalize(<direction>)
    And r ← ray(<point>, direction)
  When xs ← local_intersect(cyl, r)
  Then xs.count = <count>

  Examples:
    |   | point             | direction         | count |
    | 1 | point(0, 1.5, 0)  | vector(0.1, 1, 0) | 0     |
    | 2 | point(0, 3, -5)   | vector(0, 0, 1)   | 0     |
    | 3 | point(0, 0, -5)   | vector(0, 0, 1)   | 0     |
    | 4 | point(0, 2, -5)   | vector(0, 0, 1)   | 0     |
    | 5 | point(0, 1, -5)   | vector(0, 0, 1)   | 0     |
    | 6 | point(0, 1.5, -2) | vector(0, 0, 1)   | 2     |
*/
TEST(cylinder, should_intersect_constrained_the_cylinder)
{
    auto c{ Cylinder::create() };
    c->minimum = 1;
    c->maximum = 2;
    std::vector<tuple_t> points{
        tuple_t::point(0, 1.5, 0),
        tuple_t::point(0, 3, -5),
        tuple_t::point(0, 0, -5),
        tuple_t::point(0, 2, -5),
        tuple_t::point(0, 1, -5),
        tuple_t::point(0, 1.5, -2)
    };
    std::vector<tuple_t> directions{
        tuple_t::vector(0.1, 1, 0),
        tuple_t::vector(0, 0, 1),
        tuple_t::vector(0, 0, 1),
        tuple_t::vector(0, 0, 1),
        tuple_t::vector(0, 0, 1),
        tuple_t::vector(0, 0, 1),
    };
    std::size_t index{ 0 };
    intersections_t i{};
    for (const auto& p : points)
    {
        tuple_t direction{ directions[index] };
        direction.normalize();
        const ray_t r{ p, direction };
        c->intersect(r, i);
        if (index == 5)
        {
            EXPECT_EQ(i.entries.size(), 2);
        }
        else
        {
            EXPECT_EQ(i.entries.size(), 0);
        }
        i.entries.clear();
        index++;
    }
}

/*
Scenario: The default closed value for a cylinder
  Given cyl ← cylinder()
  Then cyl.closed = false
*/
TEST(cylinder, should_have_default_closed_value)
{
    const auto c{ Cylinder::create() };
    EXPECT_FALSE(c->closed);
}

/*
Scenario Outline: Intersecting the caps of a closed cylinder
  Given cyl ← cylinder()
    And cyl.minimum ← 1
    And cyl.maximum ← 2
    And cyl.closed ← true
    And direction ← normalize(<direction>)
    And r ← ray(<point>, direction)
  When xs ← local_intersect(cyl, r)
  Then xs.count = <count>

  Examples:
    |   | point            | direction        | count |
    | 1 | point(0, 3, 0)   | vector(0, -1, 0) | 2     |
    | 2 | point(0, 3, -2)  | vector(0, -1, 2) | 2     |
    | 3 | point(0, 4, -2)  | vector(0, -1, 1) | 2     | # corner case
    | 4 | point(0, 0, -2)  | vector(0, 1, 2)  | 2     |
    | 5 | point(0, -1, -2) | vector(0, 1, 1)  | 2     | # corner case
*/
TEST(cylinder, should_intersect_capped_cylinder)
{
    auto c{ Cylinder::create() };
    c->minimum = 1;
    c->maximum = 2;
    c->closed = true;
    std::vector<tuple_t> points{
        tuple_t::point(0, 3, 0),
        tuple_t::point(0, 3, -2),
        tuple_t::point(0, 4, -2),
        tuple_t::point(0, 0, -2),
        tuple_t::point(0, -1, -2)
    };
    std::vector<tuple_t> directions{
        tuple_t::vector(0, -1, 0),
        tuple_t::vector(0, -1, 2),
        tuple_t::vector(0, -1, 1),
        tuple_t::vector(0, 1, 2),
        tuple_t::vector(0, 1, 1)
    };
    std::size_t index{ 0 };
    intersections_t i{};
    for (const auto& p : points)
    {
        tuple_t direction{ directions[index] };
        direction.normalize();
        const ray_t r{ p, direction };
        c->intersect(r, i);
        EXPECT_EQ(i.entries.size(), 2);
        i.entries.clear();
        index++;
    }
}

/*
Scenario Outline: The normal vector on a cylinder's end caps
  Given cyl ← cylinder()
    And cyl.minimum ← 1
    And cyl.maximum ← 2
    And cyl.closed ← true
  When n ← local_normal_at(cyl, <point>)
  Then n = <normal>

  Examples:
    | point            | normal           |
    | point(0, 1, 0)   | vector(0, -1, 0) |
    | point(0.5, 1, 0) | vector(0, -1, 0) |
    | point(0, 1, 0.5) | vector(0, -1, 0) |
    | point(0, 2, 0)   | vector(0, 1, 0)  |
    | point(0.5, 2, 0) | vector(0, 1, 0)  |
    | point(0, 2, 0.5) | vector(0, 1, 0)  |
*/
TEST(cylinder, should_calculate_normal_at_cylinder_end_caps)
{
    auto c{ Cylinder::create() };
    c->minimum = 1;
    c->maximum = 2;
    c->closed = true;
    std::vector<tuple_t> points{
        tuple_t::point(0, 1, 0),
        tuple_t::point(0.5, 1, 0),
        tuple_t::point(0, 1, 0.5),
        tuple_t::point(0, 2, 0),
        tuple_t::point(0.5, 2, 0),
        tuple_t::point(0, 2, 0.5),
    };
    std::vector<tuple_t> normals{
        tuple_t::vector(0, -1, 0),
        tuple_t::vector(0, -1, 0),
        tuple_t::vector(0, -1, 0),
        tuple_t::vector(0, 1, 0),
        tuple_t::vector(0, 1, 0),
        tuple_t::vector(0, 1, 0)
    };
    std::size_t index{ 0 };
    for (const auto& p : points)
    {
        tuple_t direction{ normals[index] };
        direction.normalize();
        const tuple_t n{ c->normal_at(p) };
        EXPECT_EQ(n, normals[index]);
        index++;
    }
}

/*
Scenario: An unbounded cylinder has a bounding box
  Given shape ← cylinder()
  When box ← bounds_of(shape)
  Then box.min = point(-1, -infinity, -1)
    And box.max = point(1, infinity, 1)
*/
TEST(cylinder, should_have_bounding_box)
{
    const auto c{ Cylinder::create() };
    const bbox_t box{ c->bounds() };
    EXPECT_EQ(box.min.x, -1);
    EXPECT_EQ(box.min.y, -INFINITY);
    EXPECT_EQ(box.min.z, -1);
    EXPECT_EQ(box.max.x, 1);
    EXPECT_EQ(box.max.y, INFINITY);
    EXPECT_EQ(box.max.z, 1);
    
}

/*
Scenario: A bounded cylinder has a bounding box
  Given shape ← cylinder()
    And shape.minimum ← -5
    And shape.maximum ← 3
  When box ← bounds_of(shape)
  Then box.min = point(-1, -5, -1)
    And box.max = point(1, 3, 1)
*/
TEST(cylinder, should_have_bounding_box_for_bounded_cylinder)
{
    auto c{ Cylinder::create() };
    c->minimum = -5;
    c->maximum = 3;
    const bbox_t box{ c->bounds() };
    EXPECT_EQ(box.min, tuple_t::point(-1, -5, -1));
    EXPECT_EQ(box.max, tuple_t::point(1, 3, 1));
}