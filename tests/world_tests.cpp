#include <memory>
#include "gtest/gtest.h"
#include "../world.h"
#include "../matrix.h"
#include "../phong.h"
#include "../colour.h"
#include "../ray.h"
#include "../intersection.h"
#include "../light.h"
#include "../sphere.h"
#include "../plane.h"

class TestPattern : public Pattern
{
public:
    TestPattern(const colour_t& a, const colour_t& b)
        : Pattern{ a, b }
    {
    }
    colour_t at(const tuple_t& point) const
    {
        return { point.x, point.y, point.z };
    }
};

/*
Scenario: Creating a world
  Given w ← world()
  Then w contains no objects
	And w has no light source
*/
TEST(world, should_create_empty_world)
{
	const World w{};
	EXPECT_EQ(w.scene_objects.size(), 0);
	EXPECT_EQ(w.lights.size(), 0);
}

/*
Scenario: The default world
  Given light ← light(color(1, 1, 1))
    And light.transform = translation(-10, 10, -10)
    And s1 ← sphere() with:
      | material.color     | (0.8, 1.0, 0.6)        |
      | material.diffuse   | 0.7                    |
      | material.specular  | 0.2                    |
    And s2 ← sphere() with:
      | transform | scaling(0.5, 0.5, 0.5) |
  When w ← default_world()
  Then w.lights contains light
    And w.renderables contains s1
    And w renderablescontains s2
*/
TEST(world, should_create_default_world)
{
    const World w{ World::default_world()};
    Phong p{};
    p.colour = colour_t{ 0.8, 1.0, 0.6 };
    p.diffuse = 0.7;
    p.specular = 0.2;
    Phong v2{};
    Light l{};
    l.transform = matrix_t::translation(-10, 10, -10);
    auto assigned_phong1{ std::dynamic_pointer_cast<Phong>(w.renderables[0].lock()->material) };
    auto assigned_phong2{ std::dynamic_pointer_cast<Phong>(w.renderables[1].lock()->material) };
    EXPECT_EQ(w.scene_objects.size(), 3);
    EXPECT_EQ(w.lights.size(), 1);
    EXPECT_EQ(w.renderables[0].lock().get()->transform, matrix_t::identity());
    EXPECT_EQ(*assigned_phong1, p);
    EXPECT_EQ(w.renderables[1].lock().get()->transform, matrix_t::scaling(0.5, 0.5, 0.5));
    EXPECT_EQ(*assigned_phong2, v2);
    EXPECT_EQ(w.lights[0].lock().get()->intensity, l.intensity);
    EXPECT_EQ(w.lights[0].lock().get()->transform, l.transform);
}

/*
Scenario: Intersect a world with a ray
  Given w ← default_world()
    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And xs ← intersections()
  When xs ← w.intersect(r, xs)
  Then xs.count = 4
    And xs[0].t = 4
    And xs[1].t = 4.5
    And xs[2].t = 5.5
    And xs[3].t = 6
*/
TEST(world, should_intersect_all_renderable_objects_with_ray)
{
    const World w{ World::default_world() };
    const tuple_t origin{ tuple_t::point(0, 0, -5) };
    const tuple_t direction{ tuple_t::vector(0, 0, 1) };
    const ray_t r{ origin, direction };
    intersections_t intersections{};
    w.intersect(r, intersections);
    EXPECT_EQ(intersections.entries.size(), 4);
    EXPECT_EQ(intersections[0].time, 4);
    EXPECT_EQ(intersections[1].time, 4.5);
    EXPECT_EQ(intersections[2].time, 5.5);
    EXPECT_EQ(intersections[3].time, 6);
}

/*
Scenario: Shading an intersection
  Given w ← default_world()
    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And shape ← the first object in w
    And i ← intersection(4, shape)
  When state ← i.prepare(r)
    And c ← w.shade_hit(state)
  Then c = colour(0.38066, 0.47583, 0.2855)
*/
TEST(world, should_shade_an_intersection)
{
    World w{ World::default_world() };
    const tuple_t origin{ tuple_t::point(0, 0, -5) };
    const tuple_t direction{ tuple_t::vector(0, 0, 1) };
    const ray_t r{ origin, direction };
    const intersections_t intersections{};
    const intersection_t i{ 4, w.renderables[0].lock() };
    const intersection_state state{ i.prepare(r, intersections) };
    const colour_t c{ 0.38066, 0.47583, 0.2855 };
    EXPECT_EQ(w.shade_hit(state, 0), c);
}

/*
Scenario: Shading an intersection from the inside
  Given w ← default_world()
    And w.light ← point_light(point(0, 0.25, 0), color(1, 1, 1))
    And r ← ray(point(0, 0, 0), vector(0, 0, 1))
    And shape ← the second object in w
    And i ← intersection(0.5, shape)
  When state ← i.prepare(r)
    And c ← w.shade_hit(state)
  Then c = colour(0.90498, 0.90498, 0.90498)
*/
TEST(world, should_shade_an_intersection_from_the_inside)
{
    World w{ World::default_world() };
    w.lights[0].lock().get()->transform = matrix_t::translation(0, 0.25, 0);
    const tuple_t origin{ tuple_t::point(0, 0, 0) };
    const tuple_t direction{ tuple_t::vector(0, 0, 1) };
    const ray_t r{ origin, direction };
    const intersections_t intersections{};
    const intersection_t i{ 0.5, w.renderables[1].lock() };
    const intersection_state state{ i.prepare(r, intersections) };
    const colour_t c{ 0.90498, 0.90498, 0.90498 };
    EXPECT_EQ(w.shade_hit(state, 0), c);
}

/*
Scenario: The color when a ray misses
  Given w ← default_world()
    And r ← ray(point(0, 0, -5), vector(0, 1, 0))
  When c ← w.colour_at(r)
  Then c = colour(0, 0, 0)
*/
TEST(world, should_be_black_if_ray_misses)
{
    World w{ World::default_world() };
    const tuple_t origin{ tuple_t::point(0, 0, -5) };
    const tuple_t direction{ tuple_t::vector(0, 1, 0) };
    const ray_t r{ origin, direction };
    const colour_t c{ 0, 0, 0 };
    EXPECT_EQ(w.colour_at(r, 0), c);
}

/*
Scenario: The color when a ray hits
  Given w ← default_world()
    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
  When c ← w.colour_at(r)
  Then c = colour(0.38066, 0.47583, 0.2855)
*/
TEST(world, should_return_colour_when_a_ray_hits)
{
    World w{ World::default_world() };
    const tuple_t origin{ tuple_t::point(0, 0, -5) };
    const tuple_t direction{ tuple_t::vector(0, 0, 1) };
    const ray_t r{ origin, direction };
    const colour_t c{ 0.38066, 0.47583, 0.2855 };
    EXPECT_EQ(w.colour_at(r, 0), c);
}

/*
Scenario: The color with an intersection behind the ray
  Given w ← default_world()
    And outer ← the first object in w
    And outer.material.ambient ← 1
    And inner ← the second object in w
    And inner.material.ambient ← 1
    And r ← ray(point(0, 0, 0.75), vector(0, 0, -1))
  When c ← w.color_at(r)
  Then c = inner.material.color
*/
TEST(world, should_return_colour_when_intersection_is_behind_the_ray)
{
    World w{ World::default_world() };
    colour_t c{ 0, 0, 0 };
    auto phong = std::dynamic_pointer_cast<Phong>(w.renderables[0].lock().get()->material);
    if (phong) {
        phong->ambient = 1;
    }
    phong = std::dynamic_pointer_cast<Phong>(w.renderables[1].lock().get()->material);
    if (phong) {
        phong->ambient = 1;
        c = phong->colour;
    }
    const tuple_t origin{ tuple_t::point(0, 0, 0.75) };
    const tuple_t direction{ tuple_t::vector(0, 0, -1) };
    const ray_t r{ origin, direction };
    EXPECT_EQ(w.colour_at(r, 0), c);
}

/*
Scenario: There is no shadow when nothing is collinear with point and light
  Given w ← default_world()
    And p ← point(0, 10, 0)
   Then w.is_shadowed(p) is false
*/
TEST(world, should_return_false_when_nothing_is_collinear_with_point_and_light)
{
    World w{ World::default_world() };
    const tuple_t p{ tuple_t::point(0, 10, 0) };
    EXPECT_FALSE(w.is_shadowed(p, w.lights[0]));
}

/*
Scenario: The shadow when an object is between the point and the light
  Given w ← default_world()
    And p ← point(10, -10, 10)
   Then w.is_shadowed(p) is true
*/
TEST(world, should_return_true_when_object_between_point_and_light)
{
    World w{ World::default_world() };
    const tuple_t p{ tuple_t::point(10, -10, 10) };
    EXPECT_TRUE(w.is_shadowed(p, w.lights[0]));
}

/*
Scenario: There is no shadow when an object is behind the light
  Given w ← default_world()
    And p ← point(-20, 20, -20)
   Then w.is_shadowed(p) is false
*/
TEST(world, should_return_false_when_object_is_behind_the_light)
{
    World w{ World::default_world() };
    const tuple_t p{ tuple_t::point(-20, 20, -20) };
    EXPECT_FALSE(w.is_shadowed(p, w.lights[0]));
}

/*
Scenario: There is no shadow when an object is behind the point
  Given w ← default_world()
    And p ← point(-2, 2, -2)
   Then w.is_shadowed(p) is false
*/
TEST(world, should_return_false_when_object_is_behind_the_point)
{
    World w{ World::default_world() };
    const tuple_t p{ tuple_t::point(-2, 2, -2) };
    EXPECT_FALSE(w.is_shadowed(p, w.lights[0]));
}

/*
Scenario: shade_hit() is given an intersection in shadow
  Given w ← world()
    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
    And light is added to w
    And s1 ← sphere()
    And s1 is added to w
    And s2 ← sphere() with:
      | transform | translation(0, 0, 10) |
    And s2 is added to w
    And r ← ray(point(0, 0, 5), vector(0, 0, 1))
    And i ← intersection(4, s2)
  When state ← i.prepare(r)
    And c ← w.shade_hit(state)
  Then c = color(0.1, 0.1, 0.1)
*/
TEST(world, should_handle_in_shadow_in_shade_hit_func)
{
    World w{};
    Light light{ colour_t{1, 1, 1} };
    light.transform = matrix_t::translation(0, 0, -10);
    auto s1{ Sphere::create() };
    auto s2{ Sphere::create() };
    s2->transform = matrix_t::translation(0, 0, 10);
    w.add_object(std::make_unique<Light>(light));
    w.add_object(std::move(s1));
    w.add_object(std::move(s2));
    const ray_t r{ tuple_t::point(0, 0, 5), tuple_t::vector(0, 0, 1) };
    const intersections_t intersections{};
    const intersection_t i{ 4, w.renderables[1].lock() };
    const intersection_state state{ i.prepare(r, intersections) };
    const colour_t c{ 0.1, 0.1, 0.1 };
    EXPECT_EQ(w.shade_hit(state, 0), c);
}

/*
Scenario: The reflected color for a nonreflective material
  Given w ← default_world()
    And r ← ray(point(0, 0, 0), vector(0, 0, 1))
    And shape ← the second object in w
    And shape.material.ambient ← 1
    And i ← intersection(1, shape)
  When state ← i.prepare(r)
    And color ← w.reflected_colour(state)
  Then color = color(0, 0, 0)
*/
TEST(world, should_return_reflected_colour_as_black_for_nonreflective_material)
{
    World w{ World::default_world() };
    auto phong{ std::dynamic_pointer_cast<Phong>(w.renderables[1].lock().get()->material) };
    phong->ambient = 1;
    const ray_t r{ tuple_t::point(0, 0, 5), tuple_t::vector(0, 0, 1) };
    const intersections_t intersections{};
    const intersection_t i{ 1, w.renderables[1].lock() };
    const intersection_state state{ i.prepare(r, intersections) };
    const colour_t c{ 0, 0, 0 };
    EXPECT_EQ(w.reflected_colour(state, 0), c);
}

/*
Scenario: The reflected color for a reflective material
  Given w ← default_world()
    And shape ← plane() with:
      | material.reflective | 0.5                   |
      | transform           | translation(0, -1, 0) |
    And shape is added to w
    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
    And i ← intersection(√2, shape)
  When state ← i.prepare(r)
    And color ← w.reflected_color(state)
  Then color = color(0.19032, 0.2379, 0.14274)
*/
TEST(world, should_return_reflected_colour_for_reflective_material)
{
    World w{ World::default_world() };
    auto plane{ Plane::create() };
    plane->transform = matrix_t::translation(0, -1, 0);
    w.add_object(std::move(plane));
    auto phong{ std::dynamic_pointer_cast<Phong>(w.renderables[2].lock().get()->material) };
    phong->reflective = 0.5;
    const ray_t r{ tuple_t::point(0, 0, -3), tuple_t::vector(0, -std::sqrt(2) / 2, std::sqrt(2) / 2) };
    const intersections_t intersections{};
    const intersection_t i{ std::sqrt(2), w.renderables[2].lock() };
    const intersection_state state{ i.prepare(r, intersections) };
    const colour_t c{ 0.19032, 0.2379, 0.14274 };
    EXPECT_EQ(w.reflected_colour(state, 1), c);
}

/*
Scenario: shade_hit() with a reflective material
  Given w ← default_world()
    And shape ← plane() with:
      | material.reflective | 0.5                   |
      | transform           | translation(0, -1, 0) |
    And shape is added to w
    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
    And i ← intersection(√2, shape)
  When state ← i.prepare(r)
    And color ← w.shade_hit(state)
  Then color = color(0.87677, 0.92436, 0.82918)
*/
TEST(world, should_shade_hit_with_reflected_colour)
{
    World w{ World::default_world() };
    auto plane{ Plane::create() };
    plane->transform = matrix_t::translation(0, -1, 0);
    w.add_object(std::move(plane));
    auto phong{ std::dynamic_pointer_cast<Phong>(w.renderables[2].lock().get()->material) };
    phong->reflective = 0.5;
    const ray_t r{ tuple_t::point(0, 0, -3), tuple_t::vector(0, -std::sqrt(2) / 2, std::sqrt(2) / 2) };
    const intersections_t intersections{};
    const intersection_t i{ std::sqrt(2), w.renderables[2].lock() };
    const intersection_state state{ i.prepare(r, intersections) };
    const colour_t c{ 0.87677, 0.92436, 0.82918 };
    EXPECT_EQ(w.shade_hit(state, 1), c);
}

/*
Scenario: color_at() with mutually reflective surfaces
  Given w ← world()
    And w.light ← point_light(point(0, 0, 0), color(1, 1, 1))
    And lower ← plane() with:
      | material.reflective | 1                     |
      | transform           | translation(0, -1, 0) |
    And lower is added to w
    And upper ← plane() with:
      | material.reflective | 1                    |
      | transform           | translation(0, 1, 0) |
    And upper is added to w
    And r ← ray(point(0, 0, 0), vector(0, 1, 0))
  Then w.color_at(r) should terminate successfully
*/
TEST(world, should_terminate_successfully_for_mutually_reflective_surfaces)
{
    World w{};
    Light light{ colour_t{ 1, 1, 1 } };
    w.add_object(std::make_unique<Light>(light));
    auto plane{ Plane::create() };
    plane->transform = matrix_t::translation(0, -1, 0);
    w.add_object(std::move(plane));
    auto phong{ std::dynamic_pointer_cast<Phong>(w.renderables[0].lock().get()->material) };
    phong->reflective = 1;
    auto plane2{ Plane::create() };
    plane2->transform = matrix_t::translation(0, 1, 0);
    w.add_object(std::move(plane2));
    phong = std::dynamic_pointer_cast<Phong>(w.renderables[1].lock().get()->material);
    phong->reflective = 1;
    const ray_t r{ tuple_t::point(0, 0, 0), tuple_t::vector(0, 1, 0) };
    EXPECT_NO_THROW(w.colour_at(r, 0));
}

/*
Scenario: The reflected color at the maximum recursive depth
  Given w ← default_world()
    And shape ← plane() with:
      | material.reflective | 0.5                   |
      | transform           | translation(0, -1, 0) |
    And shape is added to w
    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
    And i ← intersection(√2, shape)
  When comps ← i.prepare(r)
    And color ← w.reflected_color(comps, 0)
  Then color = color(0, 0, 0)
*/
TEST(world, should_return_reflected_colour_at_the_maximum_recursive_depth)
{
    World w{ World::default_world() };
    auto plane{ Plane::create() };
    plane->transform = matrix_t::translation(0, -1, 0);
    w.add_object(std::move(plane));
    auto phong{ std::dynamic_pointer_cast<Phong>(w.renderables[2].lock().get()->material) };
    phong->reflective = 0.5;
    const ray_t r{ tuple_t::point(0, 0, -3), tuple_t::vector(0, -std::sqrt(2) / 2, std::sqrt(2) / 2) };
    const intersections_t intersections{};
    const intersection_t i{ std::sqrt(2), w.renderables[2].lock() };
    const intersection_state state{ i.prepare(r, intersections) };
    const colour_t c{ 0, 0, 0 };
    EXPECT_EQ(w.reflected_colour(state, 0), c);
}

/*
Scenario: The refracted color with an opaque surface
  Given w ← default_world()
    And shape ← the first object in w
    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And xs ← intersections(4:shape, 6:shape)
  When comps ← xs[0].prepare_computations(r, xs)
    And c ← w.refracted_color(comps, 5)
  Then c = color(0, 0, 0)
*/
TEST(world, should_return_refracted_colour_as_black_for_opaque_material)
{
    World w{ World::default_world() };
    const ray_t r{ tuple_t::point(0, 0, -5), tuple_t::vector(0, 0, 1) };
    intersections_t intersections{};
    const intersection_t i{ 4, w.renderables[0].lock() };
    const intersection_t i2{ 6, w.renderables[0].lock() };
    intersections.add(i, i2);
    const intersection_state state{ intersections[0].prepare(r, intersections) };
    const colour_t c{ 0, 0, 0 };
    EXPECT_EQ(w.refracted_colour(state, 5), c);
}

/*
Scenario: The refracted color at the maximum recursive depth
  Given w ← default_world()
    And shape ← the first object in w
    And shape has:
      | material.transparency     | 1.0 |
      | material.refractive_index | 1.5 |
    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And xs ← intersections(4:shape, 6:shape)
  When comps ← xs[0].prepare(r, xs)
    And c ← w.refracted_color(comps, 0)
  Then c = color(0, 0, 0)
*/
TEST(world, should_return_refracted_colour_as_black_at_the_maximum_recursive_depth)
{
    World w{ World::default_world() };
    auto phong{ std::dynamic_pointer_cast<Phong>(w.renderables[0].lock().get()->material) };
    if (phong)
    {
        phong->transparency = 1.0;
        phong->refractive_index = 1.5;
    }
    const ray_t r{ tuple_t::point(0, 0, -5), tuple_t::vector(0, 0, 1) };
    intersections_t intersections{};
    const intersection_t i{ 4, w.renderables[0].lock() };
    const intersection_t i2{ 6, w.renderables[0].lock() };
    intersections.add(i, i2);
    const intersection_state state{ intersections[0].prepare(r, intersections) };
    const colour_t c{ 0, 0, 0 };
    EXPECT_EQ(w.refracted_colour(state, 0), c);
}

/*
Scenario: The refracted color under total internal reflection
  Given w ← default_world()
    And shape ← the first object in w
    And shape has:
      | material.transparency     | 1.0 |
      | material.refractive_index | 1.5 |
    And r ← ray(point(0, 0, √2/2), vector(0, 1, 0))
    And xs ← intersections(-√2/2:shape, √2/2:shape)
  # NOTE: this time you're inside the sphere, so you need
  # to look at the second intersection, xs[1], not xs[0]
  When comps ← xs[1].prepare(r, xs)
    And c ← w.refracted_color(comps, 5)
  Then c = color(0, 0, 0)
*/
TEST(world, should_return_refracted_colour_as_black_for_total_internal_reflection)
{
    World w{ World::default_world() };
    auto phong{ std::dynamic_pointer_cast<Phong>(w.renderables[0].lock().get()->material) };
    if (phong)
    {
        phong->transparency = 1.0;
        phong->refractive_index = 1.5;
    }
    const ray_t r{ tuple_t::point(0, 0, std::sqrt(2) / 2), tuple_t::vector(0, 1, 0) };
    intersections_t intersections{};
    const intersection_t i{ -std::sqrt(2) / 2, w.renderables[0].lock() };
    const intersection_t i2{ std::sqrt(2) / 2, w.renderables[0].lock() };
    intersections.add(i, i2);
    const intersection_state state{ intersections[1].prepare(r, intersections) };
    const colour_t c{ 0, 0, 0 };
    EXPECT_EQ(w.refracted_colour(state, 5), c);
}

/*
Scenario: The refracted color with a refracted ray
  Given w ← default_world()
    And A ← the first object in w
    And A has:
      | material.ambient | 1.0            |
      | material.pattern | test_pattern() |
    And B ← the second object in w
    And B has:
      | material.transparency     | 1.0 |
      | material.refractive_index | 1.5 |
    And r ← ray(point(0, 0, 0.1), vector(0, 1, 0))
    And xs ← intersections(-0.9899:A, -0.4899:B, 0.4899:B, 0.9899:A)
  When comps ← xs[2].prepare_computations(r, xs)
    And c ← w.refracted_color(comps, 5)
  Then c = color(0, 0.99888, 0.04725)
*/
TEST(world, should_return_refracted_colour)
{
    World w{ World::default_world() };
    TestPattern p{ {0, 0, 0}, {1, 1, 1} };
    auto phong{ std::dynamic_pointer_cast<Phong>(w.renderables[0].lock().get()->material) };
    if (phong)
    {
        phong->ambient = 1;
        phong->pattern = std::make_shared<TestPattern>(p);
    }
    phong = std::dynamic_pointer_cast<Phong>(w.renderables[1].lock().get()->material);
    if (phong)
    {
        phong->transparency = 1.0;
        phong->refractive_index = 1.5;
    }
    const ray_t r{ tuple_t::point(0, 0, 0.1), tuple_t::vector(0, 1, 0) };
    intersections_t intersections{};
    const intersection_t i{ -0.9899, w.renderables[0].lock() };
    const intersection_t i2{ -0.4899, w.renderables[1].lock() };
    const intersection_t i3{ 0.4899, w.renderables[1].lock() };
    const intersection_t i4{ 0.9899, w.renderables[0].lock() };
    intersections.add(i, i2, i3, i4);
    const intersection_state state{ intersections[2].prepare(r, intersections) };
    const colour_t c{ 0, 0.99888, 0.04725 };
    EXPECT_EQ(w.refracted_colour(state, 5), c);
}

/*
Scenario: shade_hit() with a transparent material
  Given w ← default_world()
    And floor ← plane() with:
      | transform                 | translation(0, -1, 0) |
      | material.transparency     | 0.5                   |
      | material.refractive_index | 1.5                   |
    And floor is added to w
    And ball ← sphere() with:
      | material.color     | (1, 0, 0)                  |
      | material.ambient   | 0.5                        |
      | transform          | translation(0, -3.5, -0.5) |
    And ball is added to w
    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
    And xs ← intersections(√2:floor)
  When comps ← xs[0].prepare(r, xs)
    And color ← w.shade_hit(comps, 5)
  Then color = color(0.93642, 0.68642, 0.68642)
*/
TEST(world, should_return_colour_for_shade_hit_with_transparent_material)
{
    World w{ World::default_world() };
    auto p{ Plane::create() };
    p->transform = matrix_t::translation(0, -1, 0);
    auto phong{ std::dynamic_pointer_cast<Phong>(p->material) };
    if (phong)
    {
        phong->transparency = 0.5;
        phong->refractive_index = 1.5;

    }
    w.add_object(std::move(p));
    auto s{ Sphere::create() };
    phong = std::dynamic_pointer_cast<Phong>(s->material);
    if (phong)
    {
        phong->colour = colour_t{ 1, 0, 0 };
        phong->ambient = 0.5;
    }
    s->transform = matrix_t::translation(0, -3.5, -0.5);
    w.add_object(std::move(s));
    const ray_t r{ tuple_t::point(0, 0, -3), tuple_t::vector(0, -std::sqrt(2) / 2, std::sqrt(2) / 2) };
    intersections_t intersections{};
    const intersection_t i{ std::sqrt(2), w.renderables[2].lock() };
    intersections.add(i);
    const intersection_state state{ intersections[0].prepare(r, intersections) };
    const colour_t c{ 0.93642, 0.68642, 0.68642 };
    EXPECT_EQ(w.shade_hit(state, 5), c);
}

/*
Scenario: shade_hit() with a reflective, transparent material
  Given w ← default_world()
    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
    And floor ← plane() with:
      | transform                 | translation(0, -1, 0) |
      | material.reflective       | 0.5                   |
      | material.transparency     | 0.5                   |
      | material.refractive_index | 1.5                   |
    And floor is added to w
    And ball ← sphere() with:
      | material.color     | (1, 0, 0)                  |
      | material.ambient   | 0.5                        |
      | transform          | translation(0, -3.5, -0.5) |
    And ball is added to w
    And xs ← intersections(√2:floor)
  When comps ← xs[0].prepare(r, xs)
    And color ← w.shade_hit(comps, 5)
  Then color = color(0.93391, 0.69643, 0.69243)
*/
TEST(world, should_return_colour_for_shade_hit_with_reflective_transparent_material)
{
    World w{ World::default_world() };
    auto p{ Plane::create() };
    p->transform = matrix_t::translation(0, -1, 0);
    auto phong{ std::dynamic_pointer_cast<Phong>(p->material) };
    if (phong)
    {
        phong->transparency = 0.5;
        phong->reflective = 0.5;
        phong->refractive_index = 1.5;

    }
    w.add_object(std::move(p));
    auto s{ Sphere::create() };
    phong = std::dynamic_pointer_cast<Phong>(s->material);
    if (phong)
    {
        phong->colour = colour_t{ 1, 0, 0 };
        phong->ambient = 0.5;
    }
    s->transform = matrix_t::translation(0, -3.5, -0.5);
    w.add_object(std::move(s));
    const ray_t r{ tuple_t::point(0, 0, -3), tuple_t::vector(0, -std::sqrt(2) / 2, std::sqrt(2) / 2) };
    intersections_t intersections{};
    const intersection_t i{ std::sqrt(2), w.renderables[2].lock() };
    intersections.add(i);
    const intersection_state state{ intersections[0].prepare(r, intersections) };
    const colour_t c{ 0.93391, 0.69643, 0.69243 };
    EXPECT_EQ(w.shade_hit(state, 5), c);
}