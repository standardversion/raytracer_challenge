#include "gtest/gtest.h"
#include "../plane.h"
#include "../intersection.h"
#include "../uv.h"

/*
Scenario: The normal of a plane is constant everywhere
  Given p ← plane()
  When n1 ← p.local_normal_at(point(0, 0, 0))
	And n2 ← p.local_normal_at(point(10, 0, -10))
	And n3 ← p.local_normal_at(point(-5, 0, 150))
  Then n1 = vector(0, 1, 0)
	And n2 = vector(0, 1, 0)
	And n3 = vector(0, 1, 0)
*/
TEST(plane, should_have_constant_normal_everywhere)
{
	const auto p{ Plane::create() };
	EXPECT_EQ(p->local_normal_at(tuple_t::point(0, 0, 0)), tuple_t::vector(0, 1, 0));
	EXPECT_EQ(p->local_normal_at(tuple_t::point(10, 0, -10)), tuple_t::vector(0, 1, 0));
	EXPECT_EQ(p->local_normal_at(tuple_t::point(-5, 0, 150)), tuple_t::vector(0, 1, 0));
}

/*
Scenario: Intersect with a ray parallel to the plane
  Given p ← plane()
	And r ← ray(point(0, 10, 0), vector(0, 0, 1))
  When xs ← p.local_intersect(r)
  Then xs is empty
*/
TEST(plane, should_have_no_intersection_when_ray_is_parallel)
{
	const auto p{ Plane::create() };
	const ray_t r{ tuple_t::point(0, 10, 0), tuple_t::vector(0, 0, 1) };
	intersections_t i{};
	p->intersect(r, i);
	EXPECT_EQ(i.entries.size(), 0);
}

/*
Scenario: Intersect with a coplanar ray
  Given p ← plane()
	And r ← ray(point(0, 0, 0), vector(0, 0, 1))
  When xs ← p.local_intersect(r)
  Then xs is empty
*/
TEST(plane, should_not_intersect_with_a_coplanar_ray)
{
	const auto p{ Plane::create() };
	const ray_t r{ tuple_t::point(0, 0, 0), tuple_t::vector(0, 0, 1) };
	intersections_t i{};
	p->intersect(r, i);
	EXPECT_EQ(i.entries.size(), 0);
}

/*
Scenario: A ray intersecting a plane from above
  Given p ← plane()
	And r ← ray(point(0, 1, 0), vector(0, -1, 0))
  When xs ← p.local_intersect(r)
  Then xs.count = 1
	And xs[0].t = 1
	And xs[0].object = p
*/
TEST(plane, should_intersect_with_a_ray_from_above)
{
	const auto p{ Plane::create() };
	const ray_t r{ tuple_t::point(0, 1, 0), tuple_t::vector(0, -1, 0) };
	intersections_t i{};
	p->intersect(r, i);
	EXPECT_EQ(i.entries.size(), 1);
	EXPECT_EQ(i[0].time, 1);
	EXPECT_EQ(i[0].object.get(), p.get());
}

/*
Scenario: A ray intersecting a plane from below
  Given p ← plane()
	And r ← ray(point(0, -1, 0), vector(0, 1, 0))
  When xs ← local_intersect(p, r)
  Then xs.count = 1
	And xs[0].t = 1
	And xs[0].object = p
*/
TEST(plane, should_intersect_with_a_ray_from_below)
{
	const auto p{ Plane::create() };
	const ray_t r{ tuple_t::point(0, -1, 0), tuple_t::vector(0, 1, 0) };
	intersections_t i{};
	p->intersect(r, i);
	EXPECT_EQ(i.entries.size(), 1);
	EXPECT_EQ(i[0].time, 1);
	EXPECT_EQ(i[0].object.get(), p.get());
}

/*
Scenario: A plane has a bounding box
  Given shape ← plane()
  When box ← bounds_of(shape)
  Then box.min = point(-infinity, 0, -infinity)
	And box.max = point(infinity, 0, infinity)
*/
TEST(plane, should_have_a_bounding_box)
{
	const auto p{ Plane::create() };
	const bbox_t box{ p->bounds() };
	EXPECT_EQ(box.max.x, INFINITY);
	EXPECT_EQ(box.max.y, 0);
	EXPECT_EQ(box.max.z, INFINITY);
	EXPECT_EQ(box.min.x, -INFINITY);
	EXPECT_EQ(box.min.y, 0);
	EXPECT_EQ(box.min.z, -INFINITY);
}

/*
Scenario: Using a planar mapping on a 3D point
  Given p ← <point>
  When (u, v) ← planar_map(p)
  Then u = <u>
	And v = <v>

  Examples:
	| point                   | u    | v    |
	| point(0.25, 0, 0.5)     | 0.25 | 0.5  |
	| point(0.25, 0, -0.25)   | 0.25 | 0.75 |
	| point(0.25, 0.5, -0.25) | 0.25 | 0.75 |
	| point(1.25, 0, 0.5)     | 0.25 | 0.5  |
	| point(0.25, 0, -1.75)   | 0.25 | 0.25 |
	| point(1, 0, -1)         | 0.0  | 0.0  |
	| point(0, 0, 0)          | 0.0  | 0.0  |
*/
TEST(plane, should_use_planar_mapping_for_a_3d_point)
{
	const auto p{ Plane::create() };
	EXPECT_EQ(p->get_uv(tuple_t::point(0.25, 0, 0.5)), uv_t(0.25, 0.5));
	EXPECT_EQ(p->get_uv(tuple_t::point(0.25, 0, -0.25)), uv_t(0.25, 0.75));
	EXPECT_EQ(p->get_uv(tuple_t::point(0.25, 0.5, -0.25)), uv_t(0.25, 0.75));
	EXPECT_EQ(p->get_uv(tuple_t::point(1.25, 0, 0.5)), uv_t(0.25, 0.5));
	EXPECT_EQ(p->get_uv(tuple_t::point(0.25, 0, -1.75)), uv_t(0.25, 0.25));
	EXPECT_EQ(p->get_uv(tuple_t::point(1, 0, -1)), uv_t(0.0, 0.0));
	EXPECT_EQ(p->get_uv(tuple_t::point(0, 0, 0)), uv_t(0.0, 0.0));
}