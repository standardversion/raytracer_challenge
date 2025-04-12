#include "gtest/gtest.h"
#include "../sphere.h"
#include "../ray.h"
#include "../intersection.h"

/*
Scenario: Creating and querying a sphere
  Given s ← sphere(1)
	And s2 ← sphere(5)
	And s3 ← sphere(4)
  Then s.radius = 1
	And s.id = 1
	And s2.radius = 5
	And s2.id = 2
	And s3.radius = 4
	And s3.id = 3
*/
TEST(sphere, should_create_spheres_with_unique_ids)
{
	const sphere_t s{ 1 };
	const sphere_t s2{ 5 };
	const sphere_t s3{ 4 };
	EXPECT_EQ(s.radius, 1);
	EXPECT_EQ(s2.radius, 5);
	EXPECT_EQ(s3.radius, 4);
	EXPECT_TRUE(s.id != s3.id != s2.id);
}

/*
Scenario: A ray intersects a sphere at two points
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
	And s ← sphere()
	And xs ← intersections()
  When xs ← s.intersect(r, intersections)
  Then xs.entries.size() = 2
	And xs[0].time = 4.0
	And xs[1].time = 6.0
*/
TEST(sphere, should_intersect_sphere_at_2_points)
{
	const tuple_t origin{ tuple_t::point(0, 0, -5) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const ray_t r{ origin, direction };
	const sphere_t s{};
	intersections_t intersections{};
	s.intersect(r, intersections);
	EXPECT_EQ(intersections.entries.size(), 2);
	EXPECT_EQ(intersections[0].time, 4.0);
	EXPECT_EQ(intersections[1].time, 6.0);
}

/*
Scenario: A ray intersects a sphere at a tangent
  Given r ← ray(point(0, 1, -5), vector(0, 0, 1))
	And s ← sphere()
  And xs ← intersections()
  When xs ← s.intersect(r, intersections)
  Then xs.entries.size() = 2
	And xs[0].time = 5.0
	And xs[1].time = 5.0
*/
TEST(sphere, should_intersect_sphere_at_tangent)
{
	const tuple_t origin{ tuple_t::point(0, 1, -5) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const ray_t r{ origin, direction };
	const sphere_t s{};
	intersections_t intersections{};
	s.intersect(r, intersections);
	EXPECT_EQ(intersections.entries.size(), 2);
	EXPECT_EQ(intersections[0].time, 5.0);
	EXPECT_EQ(intersections[1].time, 5.0);
}

/*
Scenario: A ray misses a sphere
  Given r ← ray(point(0, 2, -5), vector(0, 0, 1))
	And s ← sphere()
  And xs ← intersections()
  When xs ← s.intersect(r, intersections)
  Then xs.entries.size() = 0
*/
TEST(sphere, should_miss_sphere)
{
	const tuple_t origin{ tuple_t::point(0, 2, -5) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const ray_t r{ origin, direction };
	const sphere_t s{};
	intersections_t intersections{};
	s.intersect(r, intersections);
	EXPECT_EQ(intersections.entries.size(), 0);
}

/*
Scenario: A ray originates inside a sphere
  Given r ← ray(point(0, 0, 0), vector(0, 0, 1))
	And s ← sphere()
  And xs ← intersections()
  When xs ← s.intersect(r, intersections)
  Then xs.entries.size() = 2
	And xs[0].time = -1.0
	And xs[1].time = 1.0
*/
TEST(sphere, should_intersect_sphere_even_if_ray_originates_inside_sphere)
{
	const tuple_t origin{ tuple_t::point(0, 0, 0) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const ray_t r{ origin, direction };
	const sphere_t s{};
	intersections_t intersections{};
	s.intersect(r, intersections);
	EXPECT_EQ(intersections.entries.size(), 2);
	EXPECT_EQ(intersections[0].time, -1.0);
	EXPECT_EQ(intersections[1].time, 1.0);
}

/*
Scenario: A sphere is behind a ray
  Given r ← ray(point(0, 0, 5), vector(0, 0, 1))
	And s ← sphere()
  And xs ← intersections()
  When xs ← s.intersect(r, intersections)
  Then xs.entries.size() = 2
	And xs[0].time = -6.0
	And xs[1].time = -4.0
*/
TEST(sphere, should_intersect_sphere_even_if_ray_is_behind_sphere)
{
	const tuple_t origin{ tuple_t::point(0, 0, 5) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const ray_t r{ origin, direction };
	const sphere_t s{};
	intersections_t intersections{};
	s.intersect(r, intersections);
	EXPECT_EQ(intersections.entries.size(), 2);
	EXPECT_EQ(intersections[0].time, -6.0);
	EXPECT_EQ(intersections[1].time, -4.0);
}