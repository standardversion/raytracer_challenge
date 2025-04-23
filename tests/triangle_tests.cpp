#include "gtest/gtest.h"
#include "../triangle.h"
#include "../tuple.h"
#include "../ray.h"
#include "../intersection.h"

/*
Scenario: Constructing a triangle
  Given p1 ← point(0, 1, 0)
    And p2 ← point(-1, 0, 0)
    And p3 ← point(1, 0, 0)
    And t ← triangle(p1, p2, p3)
  Then t.p1 = p1
    And t.p2 = p2
    And t.p3 = p3
    And t.e1 = vector(-1, -1, 0)
    And t.e2 = vector(1, -1, 0)
    And t.normal = vector(0, 0, -1)
    And t.v1_uv = pair(0, 0)
    And t.v2_uv = pair(0, 0)
    And t.v3_uv = pair(0, 0)
*/
TEST(triangle, should_construct_triangle_from_3_points)
{
    const tuple_t v1{ tuple_t::point(0, 1, 0) };
    const tuple_t v2{ tuple_t::point(-1, 0, 0) };
    const tuple_t v3{ tuple_t::point(1, 0, 0) };
    std::pair<double, double> uv{ 0.0, 0.0 };
    auto t{ Triangle::create(v1, v2, v3) };
	EXPECT_EQ(t->v1, v1);
    EXPECT_EQ(t->v2, v2);
    EXPECT_EQ(t->v3, v3);
    EXPECT_EQ(t->e1, tuple_t::vector(-1, -1, 0));
    EXPECT_EQ(t->e2, tuple_t::vector(1, -1, 0));
    EXPECT_EQ(t->normal, tuple_t::vector(0, 0, -1));
    EXPECT_EQ(t->v1_uv, uv);
    EXPECT_EQ(t->v2_uv, uv);
    EXPECT_EQ(t->v3_uv, uv);
}

/*
Scenario: Constructing a triangle with points and uvs
  Given p1 ← point(0, 1, 0)
    And p2 ← point(-1, 0, 0)
    And p3 ← point(1, 0, 0)
    And uv ← pair(1, 0)
    And t ← triangle(p1, p2, p3, uv, uv, uv)
  Then t.p1 = p1
    And t.p2 = p2
    And t.p3 = p3
    And t.e1 = vector(-1, -1, 0)
    And t.e2 = vector(1, -1, 0)
    And t.normal = vector(0, 0, -1)
    And t.v1_uv = pair(1, 0)
    And t.v2_uv = pair(1, 0)
    And t.v3_uv = pair(1, 0)
*/
TEST(triangle, should_construct_triangle_from_3_points_uvs)
{
    const tuple_t v1{ tuple_t::point(0, 1, 0) };
    const tuple_t v2{ tuple_t::point(-1, 0, 0) };
    const tuple_t v3{ tuple_t::point(1, 0, 0) };
    const std::pair<double, double> uv{ 1.0, 0.0 };
    auto t{ Triangle::create(v1, v2, v3, uv, uv, uv) };
    EXPECT_EQ(t->v1, v1);
    EXPECT_EQ(t->v2, v2);
    EXPECT_EQ(t->v3, v3);
    EXPECT_EQ(t->e1, tuple_t::vector(-1, -1, 0));
    EXPECT_EQ(t->e2, tuple_t::vector(1, -1, 0));
    EXPECT_EQ(t->normal, tuple_t::vector(0, 0, -1));
    EXPECT_EQ(t->v1_uv, uv);
    EXPECT_EQ(t->v2_uv, uv);
    EXPECT_EQ(t->v3_uv, uv);
}

/*
Scenario: Finding the normal on a triangle
  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
  When n1 ← local_normal_at(t, point(0, 0.5, 0))
    And n2 ← local_normal_at(t, point(-0.5, 0.75, 0))
    And n3 ← local_normal_at(t, point(0.5, 0.25, 0))
  Then n1 = t.normal
    And n2 = t.normal
    And n3 = t.normal
*/
TEST(triangle, should_calculate_normal_at_point)
{
    const tuple_t v1{ tuple_t::point(0, 1, 0) };
    const tuple_t v2{ tuple_t::point(-1, 0, 0) };
    const tuple_t v3{ tuple_t::point(1, 0, 0) };
    auto t{ Triangle::create(v1, v2, v3) };
    EXPECT_EQ(t->local_normal_at(tuple_t::point(0, 0.5, 0)), t->normal);
    EXPECT_EQ(t->local_normal_at(tuple_t::point(-0.5, 0.75, 0)), t->normal);
    EXPECT_EQ(t->local_normal_at(tuple_t::point(1, 0, 0)), t->normal);
}

/*
Scenario: Intersecting a ray parallel to the triangle
  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
    And r ← ray(point(0, -1, -2), vector(0, 1, 0))
  When xs ← local_intersect(t, r)
  Then xs is empty
*/
TEST(triangle, should_not_have_any_intersections_when_ray_misses)
{
    const tuple_t v1{ tuple_t::point(0, 1, 0) };
    const tuple_t v2{ tuple_t::point(-1, 0, 0) };
    const tuple_t v3{ tuple_t::point(1, 0, 0) };
    auto t{ Triangle::create(v1, v2, v3) };
    const ray_t r{ tuple_t::point(0, -1, -2), tuple_t::vector(0, 1, 0) };
    intersections_t i{};
    t->local_intersect(r, i);
    EXPECT_EQ(i.entries.size(), 0);
}

/*
Scenario: A ray misses the p1-p3 edge
  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
    And r ← ray(point(1, 1, -2), vector(0, 0, 1))
  When xs ← local_intersect(t, r)
  Then xs is empty
*/
TEST(triangle, should_not_have_any_intersections_when_ray_misses_p1_p3_edge)
{
    const tuple_t v1{ tuple_t::point(0, 1, 0) };
    const tuple_t v2{ tuple_t::point(-1, 0, 0) };
    const tuple_t v3{ tuple_t::point(1, 0, 0) };
    auto t{ Triangle::create(v1, v2, v3) };
    const ray_t r{ tuple_t::point(1, 1, -2), tuple_t::vector(0, 0, 1) };
    intersections_t i{};
    t->local_intersect(r, i);
    EXPECT_EQ(i.entries.size(), 0);
}

/*
Scenario: A ray misses the p1-p2 edge
  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
    And r ← ray(point(-1, 1, -2), vector(0, 0, 1))
  When xs ← local_intersect(t, r)
  Then xs is empty
*/
TEST(triangle, should_not_have_any_intersections_when_ray_misses_p1_p2_edge)
{
    const tuple_t v1{ tuple_t::point(0, 1, 0) };
    const tuple_t v2{ tuple_t::point(-1, 0, 0) };
    const tuple_t v3{ tuple_t::point(1, 0, 0) };
    auto t{ Triangle::create(v1, v2, v3) };
    const ray_t r{ tuple_t::point(-1, 1, -2), tuple_t::vector(0, 0, 1) };
    intersections_t i{};
    t->local_intersect(r, i);
    EXPECT_EQ(i.entries.size(), 0);
}

/*
Scenario: A ray misses the p2-p3 edge
  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
    And r ← ray(point(0, -1, -2), vector(0, 0, 1))
  When xs ← local_intersect(t, r)
  Then xs is empty
*/
TEST(triangle, should_not_have_any_intersections_when_ray_misses_p2_p3_edge)
{
    const tuple_t v1{ tuple_t::point(0, 1, 0) };
    const tuple_t v2{ tuple_t::point(-1, 0, 0) };
    const tuple_t v3{ tuple_t::point(1, 0, 0) };
    auto t{ Triangle::create(v1, v2, v3) };
    const ray_t r{ tuple_t::point(0, -1, -2), tuple_t::vector(0, 0, 1) };
    intersections_t i{};
    t->local_intersect(r, i);
    EXPECT_EQ(i.entries.size(), 0);
}

/*
Scenario: A ray strikes a triangle
  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
    And r ← ray(point(0, 0.5, -2), vector(0, 0, 1))
  When xs ← local_intersect(t, r)
  Then xs.count = 1
    And xs[0].t = 2
    And xs[0].alpha = 0.5
    And xs[0].beta = .25
    And xs[0].gamma = .25
*/
TEST(triangle, should_have_one_intersection_when_ray_strikes_a_triangle)
{
    const tuple_t v1{ tuple_t::point(0, 1, 0) };
    const tuple_t v2{ tuple_t::point(-1, 0, 0) };
    const tuple_t v3{ tuple_t::point(1, 0, 0) };
    auto t{ Triangle::create(v1, v2, v3) };
    const ray_t r{ tuple_t::point(0, 0.5, -2), tuple_t::vector(0, 0, 1) };
    intersections_t i{};
    t->local_intersect(r, i);
    EXPECT_EQ(i.entries.size(), 1);
    EXPECT_EQ(i[0].time, 2);
    EXPECT_EQ(i[0].alpha, 0.5);
    EXPECT_EQ(i[0].beta, 0.25);
    EXPECT_EQ(i[0].gamma, 0.25);
}