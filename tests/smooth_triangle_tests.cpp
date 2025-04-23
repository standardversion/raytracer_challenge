#include "gtest/gtest.h"
#include "../smooth_triangle.h"
#include "../tuple.h"
#include "../ray.h"
#include "../intersection.h"

/*
Scenario: Constructing a triangle with points, uvs and normals
  Given p1 ← point(0, 1, 0)
    And p2 ← point(-1, 0, 0)
    And p3 ← point(1, 0, 0)
    And uv ← pair(1, 0)
    And n1 ← vector(1, 0, 0)
    And n2 ← vector(0, 1, 0)
    And n3 ← vector(0, 0, 1)
    And t ← triangle(p1, p2, p3, uv, uv, uv, n1, n2, n3)
  Then t.v1 = v1
    And t.v2 = v2
    And t.v3 = v3
    And t.e1 = vector(-1, -1, 0)
    And t.e2 = vector(1, -1, 0)
    And t.normal = vector(0, 0, -1)
    And t.v1_uv = pair(1, 0)
    And t.v2_uv = pair(1, 0)
    And t.v3_uv = pair(1, 0)
    And t.n1 = n1
    And t.n2 = n2
    And t.n3 = n3
*/
TEST(smooth_triangle, should_construct_smooth_triangle_from_3_points_uvs_normals)
{
    const tuple_t v1{ tuple_t::point(0, 1, 0) };
    const tuple_t v2{ tuple_t::point(-1, 0, 0) };
    const tuple_t v3{ tuple_t::point(1, 0, 0) };
    const std::pair<double, double> uv{ 1.0, 0.0 };
    const tuple_t n1{ tuple_t::vector(1, 0, 0) };
    const tuple_t n2{ tuple_t::vector(0, 1, 0) };
    const tuple_t n3{ tuple_t::vector(0, 0, 1) };
    auto t{ SmoothTriangle::create(v1, v2, v3, uv, uv, uv, n1, n2, n3) };
    EXPECT_EQ(t->v1, v1);
    EXPECT_EQ(t->v2, v2);
    EXPECT_EQ(t->v3, v3);
    EXPECT_EQ(t->e1, tuple_t::vector(-1, -1, 0));
    EXPECT_EQ(t->e2, tuple_t::vector(1, -1, 0));
    EXPECT_EQ(t->normal, tuple_t::vector(0, 0, -1));
    EXPECT_EQ(t->v1_uv, uv);
    EXPECT_EQ(t->v2_uv, uv);
    EXPECT_EQ(t->v3_uv, uv);
    EXPECT_EQ(t->n1, n1);
    EXPECT_EQ(t->n2, n2);
    EXPECT_EQ(t->n3, n3);
}

/*
Scenario: Finding the normal on a triangle
  Given p1 ← point(0, 1, 0)
    And p2 ← point(-1, 0, 0)
    And p3 ← point(1, 0, 0)
    And uv ← pair(1, 0)
    And n1 ← vector(-1, 0, 0)
    And n2 ← vector(0, -1, 0)
    And n3 ← vector(0, 0, -1)
    And t ← triangle(p1, p2, p3, uv, uv, uv, n1, n2, n3)
  When n1 ← local_normal_at(t, point(0, -0.5, 0))
    And n2 ← local_normal_at(t, point(0.5, -0.75, 0))
    And n3 ← local_normal_at(t, point(-0.5, -0.25, 0))
  Then n1 = t.normal
    And n2 = t.normal
    And n3 = t.normal
*/
TEST(smooth_triangle, should_calculate_normal_at_point)
{
    const tuple_t v1{ tuple_t::point(0, 1, 0) };
    const tuple_t v2{ tuple_t::point(-1, 0, 0) };
    const tuple_t v3{ tuple_t::point(1, 0, 0) };
    const std::pair<double, double> uv{ 1.0, 0.0 };
    const tuple_t n1{ tuple_t::vector(0.3, 0.2, 0.1) };
    const tuple_t n2{ tuple_t::vector(0, 0.8, 0) };
    const tuple_t n3{ tuple_t::vector(0.33, 0, 0.11) };
    auto t{ SmoothTriangle::create(v1, v2, v3, uv, uv, uv, n1, n2, n3) };
    EXPECT_EQ(t->local_normal_at(tuple_t::point(0, -0.5, 0), 0.5, 0.75, 0.25), tuple_t::vector(0.232499, 0.70000, 0.077499));
    EXPECT_EQ(t->local_normal_at(tuple_t::point(0.5, -0.75, 0), 0.5, 0.75, 0.25), tuple_t::vector(0.232499, 0.70000, 0.077499));
}
