#include <string>
#include <vector>
#include <cmath>
#include "gtest/gtest.h"
#include "../sphere.h"
#include "../plane.h"
#include "../intersection.h"
#include "../intersection_state.h"
#include "../settings.h"
#include "../phong.h"
#include "../triangle.h"

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
    const intersection_t intersection1{ t, s };
    const intersection_t intersection2{ t2, s };
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
    const auto ir{ intersections.hit() };
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
    const auto ir{ intersections.hit() };
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
    const auto ir{ intersections.hit() };
    EXPECT_FALSE(ir.has_value());
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
    const auto ir{ intersections.hit() };
    EXPECT_EQ(ir, i);
}

/*
Scenario: Precomputing the state of an intersection
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And shape ← sphere()
    And i ← intersection(4, shape)
  When state ← i.prepare(r)
  Then state.t = i.t
    And state.object = i.object
    And state.point = point(0, 0, -1)
    And state.eyev = vector(0, 0, -1)
    And state.normalv = vector(0, 0, -1)
*/
TEST(intersect, should_precompute_state_of_an_intersection)
{
    const tuple_t origin{ tuple_t::point(0, 0, -5) };
    const tuple_t direction{ tuple_t::vector(0, 0, 1) };
    const ray_t r{ origin, direction };
    const auto s{ Sphere::create() };
    const intersections_t intersections{};
    const intersection_t i{ 4, s };
    const intersection_state state{ i.prepare(r, intersections) };
    EXPECT_EQ(state.time, 4);
    EXPECT_EQ(state.object, i.object.get());
    EXPECT_EQ(state.point, tuple_t::point(0, 0, -1));
    EXPECT_EQ(state.eye_vector, tuple_t::vector(0, 0, -1));
    EXPECT_EQ(state.normal, tuple_t::vector(0, 0, -1));
}

/*
Scenario: The hit, when an intersection occurs on the outside
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And shape ← sphere()
    And i ← intersection(4, shape)
  When state ← i.prepare(r)
  Then state.inside = false
*/
TEST(intersect, should_set_state_inside_attr_to_false)
{
    const tuple_t origin{ tuple_t::point(0, 0, -5) };
    const tuple_t direction{ tuple_t::vector(0, 0, 1) };
    const ray_t r{ origin, direction };
    const auto s{ Sphere::create() };
    const intersections_t intersections{};
    const intersection_t i{ 4, s };
    const intersection_state state{ i.prepare(r, intersections) };
    EXPECT_FALSE(state.inside);
}

/*
Scenario: The hit, when an intersection occurs on the inside
  Given r ← ray(point(0, 0, 0), vector(0, 0, 1))
    And shape ← sphere()
    And i ← intersection(1, shape)
  When state ← prepare_computations(i, r)
  Then state.point = point(0, 0, 1)
    And state.eyev = vector(0, 0, -1)
    And state.inside = true
      # normal would have been (0, 0, 1), but is inverted!
    And state.normalv = vector(0, 0, -1)
*/
TEST(intersect, should_set_state_inside_attr_to_true)
{
    const tuple_t origin{ tuple_t::point(0, 0, 0) };
    const tuple_t direction{ tuple_t::vector(0, 0, 1) };
    const ray_t r{ origin, direction };
    const auto s{ Sphere::create() };
    const intersections_t intersections{};
    const intersection_t i{ 1, s };
    const intersection_state state{ i.prepare(r, intersections) };
    EXPECT_TRUE(state.inside);
    EXPECT_EQ(state.point, tuple_t::point(0, 0, 1));
    EXPECT_EQ(state.eye_vector, tuple_t::vector(0, 0, -1));
    EXPECT_EQ(state.normal, tuple_t::vector(0, 0, -1));
}

/*
Scenario: The hit should offset the point
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And shape ← sphere() with:
      | transform | translation(0, 0, 1) |
    And i ← intersection(5, shape)
  When state ← prepare_computations(i, r)
  Then state.over_point.z < -EPSILON/2
    And state.point.z > state.over_point.z
*/
TEST(intersect, should_offset_the_point_on_hit)
{
    const tuple_t origin{ tuple_t::point(0, 0, -5) };
    const tuple_t direction{ tuple_t::vector(0, 0, 1) };
    const ray_t r{ origin, direction };
    auto s{ Sphere::create() };
    s->transform = matrix_t::translation(0, 0, 1);
    const intersections_t intersections{};
    const intersection_t i{ 5, s };
    const intersection_state state{ i.prepare(r, intersections) };
    EXPECT_TRUE(state.over_point.z < -EPSILON / 2);
    EXPECT_TRUE(state.point.z > state.over_point.z);
}

/*
Scenario: Precomputing the reflection vector
  Given shape ← plane()
    And r ← ray(point(0, 1, -1), vector(0, -√2/2, √2/2))
    And i ← intersection(√2, shape)
  When state ← i.prepare(r)
  Then state.reflect_vector = vector(0, √2/2, √2/2)
*/
TEST(intersect, should_precompute_reflection_vector)
{
    const tuple_t origin{ tuple_t::point(0, 1, -5) };
    const tuple_t direction{ tuple_t::vector(0, -std::sqrt(2) / 2, std::sqrt(2) / 2)};
    const ray_t r{ origin, direction };
    const auto p{ Plane::create() };
    const intersections_t intersections{};
    const intersection_t i{ std::sqrt(2), p };
    const intersection_state state{ i.prepare(r, intersections) };
    EXPECT_EQ(state.reflect_vector, tuple_t::vector(0, std::sqrt(2) / 2, std::sqrt(2) / 2));
}

/*
Scenario Outline: Finding n1 and n2 at various intersections
  Given A ← glass_sphere() with:
      | transform                 | scaling(2, 2, 2) |
      | material.refractive_index | 1.5              |
    And B ← glass_sphere() with:
      | transform                 | translation(0, 0, -0.25) |
      | material.refractive_index | 2.0                      |
    And C ← glass_sphere() with:
      | transform                 | translation(0, 0, 0.25) |
      | material.refractive_index | 2.5                     |
    And r ← ray(point(0, 0, -4), vector(0, 0, 1))
    And xs ← intersections(2:A, 2.75:B, 3.25:C, 4.75:B, 5.25:C, 6:A)
  When state ← xs[<index>].prepare(r, xs)
  Then state.n1 = <n1>
    And state.n2 = <n2>

  Examples:
    | index | n1  | n2  |
    | 0     | 1.0 | 1.5 |
    | 1     | 1.5 | 2.0 |
    | 2     | 2.0 | 2.5 |
    | 3     | 2.5 | 2.5 |
    | 4     | 2.5 | 1.5 |
    | 5     | 1.5 | 1.0 |
*/
TEST(intersect, should_find_n1_and_n2_at_various_intersections)
{
    const tuple_t origin{ tuple_t::point(0, 0, -4) };
    const tuple_t direction{ tuple_t::vector(0, 0, 1) };
    const ray_t r{ origin, direction };
    auto a{ Sphere::glass_sphere() };
    a->transform = matrix_t::scaling(2, 2, 2);
    auto phong{ std::dynamic_pointer_cast<Phong>(a->material) };
    if (phong)
    {
        phong->refractive_index = 1.5;
    }
    auto b{ Sphere::glass_sphere() };
    b->transform = matrix_t::translation(0, 0, -0.25);
    phong = std::dynamic_pointer_cast<Phong>(b->material);
    if (phong)
    {
        phong->refractive_index = 2.0;
    }
    auto c{ Sphere::glass_sphere() };
    c->transform = matrix_t::translation(0, 0, 0.25);
    phong = std::dynamic_pointer_cast<Phong>(c->material);
    if (phong)
    {
        phong->refractive_index = 2.5;
    }
    const intersection_t i{ 2, a };
    const intersection_t i2{ 2.75, b };
    const intersection_t i3{ 3.25, c };
    const intersection_t i4{ 4.75, b };
    const intersection_t i5{ 5.25, c };
    const intersection_t i6{ 6, a };
    intersections_t intersections{};
    intersections.add(i, i2, i3, i4, i5, i6);
    intersection_state state{ intersections[0].prepare(r, intersections)};
    EXPECT_EQ(state.n1, 1.0);
    EXPECT_EQ(state.n2, 1.5);
    state = intersections[1].prepare(r, intersections);
    EXPECT_EQ(state.n1, 1.5);
    EXPECT_EQ(state.n2, 2.0);
    state = intersections[2].prepare(r, intersections);
    EXPECT_EQ(state.n1, 2.0);
    EXPECT_EQ(state.n2, 2.5);
    state = intersections[3].prepare(r, intersections);
    EXPECT_EQ(state.n1, 2.5);
    EXPECT_EQ(state.n2, 2.5);
    state = intersections[4].prepare(r, intersections);
    EXPECT_EQ(state.n1, 2.5);
    EXPECT_EQ(state.n2, 1.5);
    state = intersections[5].prepare(r, intersections);
    EXPECT_EQ(state.n1, 1.5);
    EXPECT_EQ(state.n2, 1.0);
}

/*
Scenario: The under point is offset below the surface
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And shape ← glass_sphere() with:
      | transform | translation(0, 0, 1) |
    And i ← intersection(5, shape)
    And xs ← intersections(i)
  When state ← i.prepare(r, xs)
  Then state.under_point.z > EPSILON/2
    And state.point.z < comps.under_point.z
*/
TEST(intersect, should_set_under_point_as_offset_below_the_surface)
{
    const tuple_t origin{ tuple_t::point(0, 0, -5) };
    const tuple_t direction{ tuple_t::vector(0, 0, 1) };
    const ray_t r{ origin, direction };
    auto s{ Sphere::glass_sphere() };
    s->transform = matrix_t::translation(0, 0, 1);
    intersections_t intersections{};
    const intersection_t i{ 5, s };
    intersections.add(i);
    const intersection_state state{ i.prepare(r, intersections) };
    EXPECT_TRUE(state.under_point.z > EPSILON / 2);
    EXPECT_TRUE(state.point.z < state.under_point.z);
}

/*
Scenario: The Schlick approximation under total internal reflection
  Given shape ← glass_sphere()
    And r ← ray(point(0, 0, √2/2), vector(0, 1, 0))
    And xs ← intersections(-√2/2:shape, √2/2:shape)
  When comps ← xs[1].prepare(r, xs)
    And reflectance ← comps.schlick()
  Then reflectance = 1.0
*/
TEST(intersect, should_calculate_schlick_approximation_under_total_internal_reflection)
{
    const tuple_t origin{ tuple_t::point(0, 0, std::sqrt(2) / 2) };
    const tuple_t direction{ tuple_t::vector(0, 1, 0) };
    const ray_t r{ origin, direction };
    auto s{ Sphere::glass_sphere() };
    const intersection_t i{ -std::sqrt(2) / 2, s };
    const intersection_t i2{ std::sqrt(2) / 2, s };
    intersections_t intersections{};
    intersections.add(i, i2);
    const intersection_state state{ intersections[1].prepare(r, intersections) };
    EXPECT_EQ(state.schlick(), 1.0);
}

/*
Scenario: The Schlick approximation with a perpendicular viewing angle
  Given shape ← glass_sphere()
    And r ← ray(point(0, 0, 0), vector(0, 1, 0))
    And xs ← intersections(-1:shape, 1:shape)
  When comps ← xs[1].prepare(r, xs)
    And reflectance ← comps.schlick()
  Then reflectance = 0.04
*/
TEST(intersect, should_calculate_schlick_approximation_with_a_perpendicular_viewing_angle)
{
    const tuple_t origin{ tuple_t::point(0, 0, 0) };
    const tuple_t direction{ tuple_t::vector(0, 1, 0) };
    const ray_t r{ origin, direction };
    auto s{ Sphere::glass_sphere() };
    const intersection_t i{ -1, s };
    const intersection_t i2{ 1, s };
    intersections_t intersections{};
    intersections.add(i, i2);
    const intersection_state state{ intersections[1].prepare(r, intersections) };
    EXPECT_NEAR(state.schlick(), 0.04, 0.0001);
}

/*
Scenario: The Schlick approximation with small angle and n2 > n1
  Given shape ← glass_sphere()
    And r ← ray(point(0, 0.99, -2), vector(0, 0, 1))
    And xs ← intersections(1.8589:shape)
  When comps ← xs[0].prepare(r, xs)
    And reflectance ← comps.schlick()
  Then reflectance = 0.48873
*/
TEST(intersect, should_calculate_schlick_approximation_with_small_angle_and_n2_greater_than_n1)
{
    const tuple_t origin{ tuple_t::point(0, 0.99, -2) };
    const tuple_t direction{ tuple_t::vector(0, 0, 1) };
    const ray_t r{ origin, direction };
    auto s{ Sphere::glass_sphere() };
    const intersection_t i{ 1.8589, s };
    intersections_t intersections{};
    intersections.add(i);
    const intersection_state state{ intersections[0].prepare(r, intersections) };
    EXPECT_NEAR(state.schlick(), 0.48873, 0.0001);
}

/*
Scenario: An intersection can encapsulate `u` and `v` and `w`
  Given s ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
  When i ← intersection_with_uv(3.5, s, 0.2, 0.4)
  Then i.u = 0.2
    And i.v = 0.4
    And i.w = 0.4
*/
TEST(intersect, should_have_u_v_w)
{

    auto t{ Triangle::create(tuple_t::point(0, 1, 0), tuple_t::point(-1, 0, 0), tuple_t::point(1, 0, 0)) };
    const intersection_t i{ 3.5, t, 0.2, 0.4, 0.4 };
    EXPECT_EQ(i.alpha, 0.2);
    EXPECT_EQ(i.beta, 0.4);
    EXPECT_EQ(i.gamma, 0.4);
}