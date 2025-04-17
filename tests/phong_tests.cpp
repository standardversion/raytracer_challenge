#include <cmath>
#include "gtest/gtest.h"
#include "../phong.h"
#include "../colour.h"
#include "../tuple.h"
#include "../light.h"
#include "../pattern.h"
#include "../sphere.h"

/*
Scenario: The default phong
  Given m ← phong()
  Then m.color = color(1, 1, 1)
    And m.ambient = 0.1
    And m.diffuse = 0.9
    And m.specular = 0.9
    And m.shininess = 200.0
*/
TEST(phong, should_create_default_material)
{
    const Phong m{};
    const colour_t c{ 1, 1, 1 };
    EXPECT_EQ(m.colour, c);
    EXPECT_EQ(m.ambient, 0.1);
    EXPECT_EQ(m.diffuse, 0.9);
    EXPECT_EQ(m.specular, 0.9);
    EXPECT_EQ(m.shininess, 200.0);
}

/*
Scenario: Lighting with the eye between the light and the surface
  Given eyev ← vector(0, 0, -1)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
  When result ← m.lighting(light, position, eyev, normalv)
  Then result = color(1.9, 1.9, 1.9)
*/
TEST(phong, should_return_colur_when_the_eye_between_the_light_and_the_surface)
{
    const auto s{ Sphere::create() };
    const Phong m{};
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    const tuple_t eye_vector{ tuple_t::vector(0, 0, -1) };
    const tuple_t normal_vector{ tuple_t::vector(0, 0, -1) };
    Light light{ colour_t{1, 1, 1} };
    light.transform = matrix_t::translation(0, 0, -10);
    const colour_t r{ 1.9, 1.9, 1.9 };
    const bool in_shadow{ false };
    EXPECT_EQ(m.lighting(light, dynamic_cast<Sphere*>(s.get()), position, eye_vector, normal_vector, in_shadow), r);
}

/*
Scenario: Lighting with the eye between light and surface, eye offset 45°
  Given eyev ← vector(0, √2/2, -√2/2)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
  When result ← m.lighting(light, position, eyev, normalv)
  Then result = color(1.0, 1.0, 1.0)
*/
TEST(phong, should_return_colur_when_the_eye_between_the_light_and_the_surface_eye_offset_45)
{
    const auto s{ Sphere::create() };
    const Phong m{};
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    const tuple_t eye_vector{ tuple_t::vector(0, std::sqrt(2) / 2, -std::sqrt(2) / 2)};
    const tuple_t normal_vector{ tuple_t::vector(0, 0, -1) };
    Light light{ colour_t{1, 1, 1} };
    light.transform = matrix_t::translation(0, 0, -10);
    const colour_t r{ 1.0, 1.0, 1.0 };
    const bool in_shadow{ false };
    EXPECT_EQ(m.lighting(light, dynamic_cast<Sphere*>(s.get()), position, eye_vector, normal_vector, in_shadow), r);
}

/*
Scenario: Lighting with eye opposite surface, light offset 45°
  Given eyev ← vector(0, 0, -1)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 10, -10), color(1, 1, 1))
  When result ← m.lighting(light, position, eyev, normalv)
  Then result = color(0.7364, 0.7364, 0.7364)
*/
TEST(phong, should_return_colur_when_the_eye_between_the_light_and_the_surface_light_offset_45)
{
    const auto s{ Sphere::create() };
    const Phong m{};
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    const tuple_t eye_vector{ tuple_t::vector(0, 0, -1) };
    const tuple_t normal_vector{ tuple_t::vector(0, 0, -1) };
    Light light{ colour_t{1, 1, 1} };
    light.transform = matrix_t::translation(0, 10, -10);
    const colour_t r{ 0.7364, 0.7364, 0.7364 };
    const bool in_shadow{ false };
    EXPECT_EQ(m.lighting(light, dynamic_cast<Sphere*>(s.get()), position, eye_vector, normal_vector, in_shadow), r);
}

/*
Scenario: Lighting with eye in the path of the reflection vector
  Given eyev ← vector(0, -√2/2, -√2/2)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 10, -10), color(1, 1, 1))
  When result ← m.lighting(light, position, eyev, normalv)
  Then result = color(1.6364, 1.6364, 1.6364)
*/
TEST(phong, should_return_colur_when_the_eye_in_the_path_of_reflection_vector)
{
    const auto s{ Sphere::create() };
    const Phong m{};
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    const tuple_t eye_vector{ tuple_t::vector(0, -std::sqrt(2) / 2, -std::sqrt(2) / 2) };
    const tuple_t normal_vector{ tuple_t::vector(0, 0, -1) };
    Light light{ colour_t{1, 1, 1} };
    light.transform = matrix_t::translation(0, 10, -10);
    const colour_t expected{ 1.6364, 1.6364, 1.6364 };
    const bool in_shadow{ false };
    const colour_t r{ m.lighting(light, dynamic_cast<Sphere*>(s.get()), position, eye_vector, normal_vector, in_shadow) };
    EXPECT_EQ(r, expected);
}

/*
Scenario: Lighting with the light behind the surface
  Given eyev ← vector(0, 0, -1)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 0, 10), color(1, 1, 1))
  When result ← m.lighting(light, position, eyev, normalv)
  Then result = color(0.1, 0.1, 0.1)
*/
TEST(phong, should_return_colur_when_the_eye_behind_the_surface)
{
    const auto s{ Sphere::create() };
    const Phong m{};
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    const tuple_t eye_vector{ tuple_t::vector(0, 0, -1) };
    const tuple_t normal_vector{ tuple_t::vector(0, 0, -1) };
    Light light{ colour_t{1, 1, 1} };
    light.transform = matrix_t::translation(0, 0, 10);
    const colour_t r{ 0.1, 0.1, 0.1 };
    const bool in_shadow{ false };
    EXPECT_EQ(m.lighting(light, dynamic_cast<Sphere*>(s.get()), position, eye_vector, normal_vector, in_shadow), r);
}

/*
Scenario: Lighting with the surface in shadow
  Given eyev ← vector(0, 0, -1)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
    And in_shadow ← true
  When result ← lighting(m, light, position, eyev, normalv, in_shadow)
  Then result = color(0.1, 0.1, 0.1)
*/
TEST(phong, should_return_colur_when_surface_in_shadow)
{
    const auto s{ Sphere::create() };
    const Phong m{};
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    const tuple_t eye_vector{ tuple_t::vector(0, 0, -1) };
    const tuple_t normal_vector{ tuple_t::vector(0, 0, -1) };
    Light light{ colour_t{1, 1, 1} };
    light.transform = matrix_t::translation(0, 0, -10);
    const colour_t r{ 0.1, 0.1, 0.1 };
    const bool in_shadow{ true };
    EXPECT_EQ(m.lighting(light, dynamic_cast<Sphere*>(s.get()), position, eye_vector, normal_vector, in_shadow), r);
}

/*
Scenario: Lighting with a pattern applied
  Given m.pattern ← stripe_pattern(color(1, 1, 1), color(0, 0, 0))
    And m.ambient ← 1
    And m.diffuse ← 0
    And m.specular ← 0
    And eyev ← vector(0, 0, -1)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
  When c1 ← lighting(m, light, point(0.9, 0, 0), eyev, normalv, false)
    And c2 ← lighting(m, light, point(1.1, 0, 0), eyev, normalv, false)
  Then c1 = color(1, 1, 1)
    And c2 = color(0, 0, 0)
*/
TEST(phong, should_take_applied_pattern_into_account)
{
    const auto s{ Sphere::create() };
    Phong m{};
    m.pattern = std::make_shared<pattern_t>(colour_t(1, 1, 1), colour_t(0, 0, 0));
    m.ambient = 1;
    m.diffuse = 0;
    m.specular = 0;
    const tuple_t eye_vector{ tuple_t::vector(0, 0, -1) };
    const tuple_t normal_vector{ tuple_t::vector(0, 0, -1) };
    Light light{ colour_t{1, 1, 1} };
    light.transform = matrix_t::translation(0, 0, -10);
    const colour_t r{ 1, 1, 1 };
    const colour_t r2{ 0, 0, 0 };
    const bool in_shadow{ false };
    EXPECT_EQ(m.lighting(light, dynamic_cast<Sphere*>(s.get()), tuple_t::point(0.9, 0, 0), eye_vector, normal_vector, in_shadow), r);
    EXPECT_EQ(m.lighting(light, dynamic_cast<Sphere*>(s.get()), tuple_t::point(1.1, 0, 0), eye_vector, normal_vector, in_shadow), r2);
}