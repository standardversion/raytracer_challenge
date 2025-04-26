#include <vector>
#include "gtest/gtest.h"
#include "../cone.h"
#include "../ray.h"
#include "../intersection.h"

/*
Scenario Outline: Intersecting a cone with a ray
  Given shape ← cone()
    And direction ← normalize(<direction>)
    And r ← ray(<origin>, direction)
  When xs ← local_intersect(shape, r)
  Then xs.count = 2
    And xs[0].t = <t0>
    And xs[1].t = <t1>

  Examples:
    | origin          | direction           | t0      | t1       |
    | point(0, 0, -5) | vector(0, 0, 1)     | 5       |  5       |
    | point(0, 0, -5) | vector(1, 1, 1)     | 8.66025 |  8.66025 |
    | point(1, 1, -5) | vector(-0.5, -1, 1) | 4.55006 | 49.44994 |
*/
TEST(cone, should_intersect_cone)
{
    const auto c{ Cone::create() };
    std::vector<tuple_t> points{
        tuple_t::point(0, 0, -5),
        tuple_t::point(0, 0, -5),
        tuple_t::point(1, 1, -5)
    };
    std::vector<tuple_t> directions{
        tuple_t::vector(0, 0, 1),
        tuple_t::vector(1, 1, 1),
        tuple_t::vector(-0.5, -1, 1)
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
        if (i.entries.size())
        {
            if (index == 0)
            {
                EXPECT_EQ(i[0].time, 5);
                EXPECT_EQ(i[1].time, 5);
            }
            else if (index == 1)
            {
                EXPECT_NEAR(i[0].time, 8.66025, 0.001);
                EXPECT_NEAR(i[1].time, 8.66025, 0.001);
            }
            else
            {
                EXPECT_NEAR(i[0].time, 4.55006, 0.001);
                EXPECT_NEAR(i[1].time, 49.44994, 0.001);
            }
            i.entries.clear();
        }
        index++;
    }
}

/*
Scenario: Intersecting a cone with a ray parallel to one of its halves
  Given shape ← cone()
    And direction ← normalize(vector(0, 1, 1))
    And r ← ray(point(0, 0, -1), direction)
  When xs ← local_intersect(shape, r)
  Then xs.count = 1
    And xs[0].t = 0.35355
*/
TEST(cone, should_intersect_cone_with_a_ray_parallel_to_one_of_its_halves)
{
    const auto c{ Cone::create() };
    std::vector<tuple_t> points{
        tuple_t::point(0, 0, -1)
    };
    std::vector<tuple_t> directions{
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
        EXPECT_EQ(i.entries.size(), 1);
        if (i.entries.size())
        {
            EXPECT_NEAR(i[0].time, 0.35355, 0.001);
            i.entries.clear();
        }
        index++;
    }
}

/*
Scenario Outline: Intersecting a cone's end caps
  Given shape ← cone()
    And shape.minimum ← -0.5
    And shape.maximum ← 0.5
    And shape.closed ← true
    And direction ← normalize(<direction>)
    And r ← ray(<origin>, direction)
  When xs ← local_intersect(shape, r)
  Then xs.count = <count>

  Examples:
    | origin             | direction       | count |
    | point(0, 0, -5)    | vector(0, 1, 0) | 0     |
    | point(0, 0, -0.25) | vector(0, 1, 1) | 2     |
    | point(0, 0, -0.25) | vector(0, 1, 0) | 4     |
*/
TEST(cone, should_intersect_cone_end_caps)
{
    auto c{ Cone::create() };
    c->minimum = -0.5;
    c->maximum = 0.5;
    c->closed = true;
    std::vector<tuple_t> points{
        tuple_t::point(0, 0, -5),
        tuple_t::point(0, 0, -0.25),
        tuple_t::point(0, 0, -0.25)
    };
    std::vector<tuple_t> directions{
        tuple_t::vector(0, 1, 0),
        tuple_t::vector(0, 1, 1),
        tuple_t::vector(0, 1, 0)
    };
    std::size_t index{ 0 };
    intersections_t i{};
    for (const auto& p : points)
    {
        tuple_t direction{ directions[index] };
        direction.normalize();
        const ray_t r{ p, direction };
        c->intersect(r, i);
        
        if (i.entries.size())
        {
            if (index == 0)
            {
                EXPECT_EQ(i.entries.size(), 0);
            }
            else if (index == 1)
            {
                EXPECT_EQ(i.entries.size(), 2);
            }
            else if (index == 2)
            {
                EXPECT_EQ(i.entries.size(), 4);
            }
            i.entries.clear();
        }
        index++;
    }
}

/*

Scenario Outline: Computing the normal vector on a cone
  Given shape ← cone()
  When n ← local_normal_at(shape, <point>)
  Then n = <normal>

  Examples:
    | point             | normal                 |
    | point(0, 0, 0)    | vector(0, 0, 0)        |
    | point(1, 1, 1)    | vector(1, -√2, 1)      |
    | point(-1, -1, 0)  | vector(-1, 1, 0)       |
*/
TEST(cone, should_compute_normal_vectors)
{
    auto c{ Cone::create() };
    std::vector<tuple_t> points{
        tuple_t::point(0, 0, 0),
        tuple_t::point(1, 1, 1),
        tuple_t::point(-1, -1, 0)
    };
    std::vector<tuple_t> normals{
        tuple_t::vector(0, 0, 0),
        tuple_t::vector(1, -std::sqrt(2), 1),
        tuple_t::vector(-1, 1, 0)
    };
    std::size_t index{ 0 };
    intersections_t i{};
    for (const auto& p : points)
    {
        const tuple_t n{ c->local_normal_at(p)};
        EXPECT_EQ(n, normals[index]);
        index++;
    }
}

/*
Scenario: An unbounded cone has a bounding box
  Given shape ← cone()
  When box ← bounds_of(shape)
  Then box.min = point(-infinity, -infinity, -infinity)
    And box.max = point(infinity, infinity, infinity)
*/
TEST(cone, should_have_bounding_box)
{
    const auto c{ Cone::create() };
    const bbox_t box{ c->bounds() };
    EXPECT_EQ(box.min.x, -INFINITY);
    EXPECT_EQ(box.min.y, -INFINITY);
    EXPECT_EQ(box.min.z, -INFINITY);
    EXPECT_EQ(box.max.x, INFINITY);
    EXPECT_EQ(box.max.y, INFINITY);
    EXPECT_EQ(box.max.z, INFINITY);
}

/*
Scenario: A bounded cone has a bounding box
  Given shape ← cone()
    And shape.minimum ← -5
    And shape.maximum ← 3
  When box ← bounds_of(shape)
  Then box.min = point(-5, -5, -5)
    And box.max = point(5, 3, 5)
*/
TEST(cone, should_have_bounding_box_for_bounded_cone)
{
    auto c{ Cone::create() };
    c->maximum = 3;
    c->minimum = -5;
    const bbox_t box{ c->bounds() };
    EXPECT_EQ(box.min, tuple_t::point(-5, -5, -5));
    EXPECT_EQ(box.max, tuple_t::point(5, 3, 5));
}