#include <memory>
#include "gtest/gtest.h"
#include "../world.h"
#include "../matrix.h"
#include "../phong.h"
#include "../colour.h"
#include "../ray.h"
#include "../intersection.h"

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
    Light l{};
    l.transform = matrix_t::translation(-10, 10, -10);
    EXPECT_EQ(w.scene_objects.size(), 3);
    EXPECT_EQ(w.lights.size(), 1);
    EXPECT_EQ(w.renderables[0]->transform, matrix_t::identity());
    EXPECT_EQ(*w.renderables[0]->material, p);
    EXPECT_EQ(w.renderables[1]->transform, matrix_t::scaling(0.5, 0.5, 0.5));
    EXPECT_EQ(*w.renderables[1]->material, p);
    EXPECT_EQ(*w.lights[0], l);
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