#include <cmath>
#include "gtest/gtest.h"
#include "../material.h"
#include "../colour.h"
#include "../tuple.h"
#include "../light.h"

/*
Scenario: The default material
  Given m ← material()
  Then m.color = color(1, 1, 1)
    And m.ambient = 0.1
    And m.diffuse = 0.9
    And m.specular = 0.9
    And m.shininess = 200.0
*/
TEST(material, should_create_default_material)
{
    const material_t m{};
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
TEST(material, should_return_colur_when_the_eye_between_the_light_and_the_surface)
{
    const material_t m{};
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    const tuple_t eye_vector{ tuple_t::vector(0, 0, -1) };
    const tuple_t normal_vector{ tuple_t::vector(0, 0, -1) };
    const light_t light{ tuple_t::point(0, 0, -10), colour_t{1, 1, 1} };
    const colour_t r{ 1.9, 1.9, 1.9 };
    EXPECT_EQ(m.lighting(light, position, eye_vector, normal_vector), r);
}

/*
Scenario: Lighting with the eye between light and surface, eye offset 45°
  Given eyev ← vector(0, √2/2, -√2/2)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
  When result ← m.lighting(light, position, eyev, normalv)
  Then result = color(1.0, 1.0, 1.0)
*/
TEST(material, should_return_colur_when_the_eye_between_the_light_and_the_surface_eye_offset_45)
{
    const material_t m{};
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    const tuple_t eye_vector{ tuple_t::vector(0, std::sqrt(2) / 2, -std::sqrt(2) / 2)};
    const tuple_t normal_vector{ tuple_t::vector(0, 0, -1) };
    const light_t light{ tuple_t::point(0, 0, -10), colour_t{1, 1, 1} };
    const colour_t r{ 1.0, 1.0, 1.0 };
    EXPECT_EQ(m.lighting(light, position, eye_vector, normal_vector), r);
}

/*
Scenario: Lighting with eye opposite surface, light offset 45°
  Given eyev ← vector(0, 0, -1)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 10, -10), color(1, 1, 1))
  When result ← m.lighting(light, position, eyev, normalv)
  Then result = color(0.7364, 0.7364, 0.7364)
*/
TEST(material, should_return_colur_when_the_eye_between_the_light_and_the_surface_light_offset_45)
{
    const material_t m{};
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    const tuple_t eye_vector{ tuple_t::vector(0, 0, -1) };
    const tuple_t normal_vector{ tuple_t::vector(0, 0, -1) };
    const light_t light{ tuple_t::point(0, 10, -10), colour_t{1, 1, 1} };
    const colour_t r{ 0.7364, 0.7364, 0.7364 };
    EXPECT_EQ(m.lighting(light, position, eye_vector, normal_vector), r);
}

/*
Scenario: Lighting with eye in the path of the reflection vector
  Given eyev ← vector(0, -√2/2, -√2/2)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 10, -10), color(1, 1, 1))
  When result ← m.lighting(light, position, eyev, normalv)
  Then result = color(1.6364, 1.6364, 1.6364)
*/
TEST(material, should_return_colur_when_the_eye_in_the_path_of_reflection_vector)
{
    const material_t m{};
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    const tuple_t eye_vector{ tuple_t::vector(0, -std::sqrt(2) / 2, -std::sqrt(2) / 2) };
    const tuple_t normal_vector{ tuple_t::vector(0, 0, -1) };
    const light_t light{ tuple_t::point(0, 10, -10), colour_t{1, 1, 1} };
    const colour_t expected{ 1.6364, 1.6364, 1.6364 };
    const colour_t r{ m.lighting(light, position, eye_vector, normal_vector) };
    EXPECT_EQ(r, expected);
}

/*
Scenario: Lighting with the light behind the surface
  Given eyev ← vector(0, 0, -1)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 0, 10), color(1, 1, 1))
  When result ← lighting(m, light, position, eyev, normalv)
  Then result = color(0.1, 0.1, 0.1)
*/
TEST(material, should_return_colur_when_the_eye_behind_the_surface)
{
    const material_t m{};
    const tuple_t position{ tuple_t::point(0, 0, 0) };
    const tuple_t eye_vector{ tuple_t::vector(0, 0, -1) };
    const tuple_t normal_vector{ tuple_t::vector(0, 0, -1) };
    const light_t light{ tuple_t::point(0, 0, 10), colour_t{1, 1, 1} };
    const colour_t r{ 0.1, 0.1, 0.1 };
    EXPECT_EQ(m.lighting(light, position, eye_vector, normal_vector), r);
}