#include "gtest/gtest.h"
#include "../tuple.h"
#include "../ray.h"
#include "../matrix.h"

/*
Scenario: Creating and querying a ray
  Given origin ← point(1, 2, 3)
	And direction ← vector(4, 5, 6)
  When r ← ray(origin, direction)
  Then r.origin = origin
	And r.direction = direction
*/
TEST(ray, should_create_a_ray_with_origin_and_direction)
{
	const tuple_t origin{ tuple_t::point(1, 2, 3) };
	const tuple_t direction{ tuple_t::vector(4, 5, 6) };
	const ray_t ray{ origin, direction };
	EXPECT_EQ(ray.origin, origin);
	EXPECT_EQ(ray.direction, direction);
}

/*
Scenario: Computing a point from a distance
  Given r ← ray(point(2, 3, 4), vector(1, 0, 0))
  Then r.position(0) = point(2, 3, 4)
	And r.position(1) = point(3, 3, 4)
	And r.position(-1) = point(1, 3, 4)
	And r.position(2.5) = point(4.5, 3, 4)
*/
TEST(ray, should_compute_a_point_from_a_distance)
{
	const tuple_t origin{ tuple_t::point(2, 3, 4) };
	const tuple_t direction{ tuple_t::vector(1, 0, 0) };
	const ray_t ray{ origin, direction };
	const tuple_t p1{ tuple_t::point(2, 3, 4) };
	const tuple_t p2{ tuple_t::point(3, 3, 4) };
	const tuple_t p3{ tuple_t::point(1, 3, 4) };
	const tuple_t p4{ tuple_t::point(4.5, 3, 4) };
	EXPECT_EQ(ray.position(0), p1);
	EXPECT_EQ(ray.position(1), p2);
	EXPECT_EQ(ray.position(-1), p3);
	EXPECT_EQ(ray.position(2.5), p4);
}

/*
Scenario: Translating a ray
  Given r ← ray(point(1, 2, 3), vector(0, 1, 0))
	And m ← translation(3, 4, 5)
  When r2 ← r.transform(m)
  Then r2.origin = point(4, 6, 8)
	And r2.direction = vector(0, 1, 0)
*/
TEST(ray, should_be_able_to_translate_a_ray)
{
	const tuple_t origin{ tuple_t::point(1, 2, 3) };
	const tuple_t direction{ tuple_t::vector(0, 1, 0) };
	const ray_t ray{ origin, direction };
	const matrix_t m{ matrix_t ::translation(3, 4, 5)};
	const tuple_t p{ tuple_t::point(4, 6, 8) };
	const tuple_t d{ tuple_t::vector(0, 1, 0) };
	const ray_t ray2{ ray.transform(m)};
	EXPECT_EQ(ray2.origin, p);
	EXPECT_EQ(ray2.direction, d);
}

/*
Scenario: Scaling a ray
  Given r ← ray(point(1, 2, 3), vector(0, 1, 0))
	And m ← scaling(2, 3, 4)
  When r2 ← r.transform(m)
  Then r2.origin = point(2, 6, 12)
	And r2.direction = vector(0, 3, 0)
*/
TEST(ray, should_be_able_to_scale_a_ray)
{
	const tuple_t origin{ tuple_t::point(1, 2, 3) };
	const tuple_t direction{ tuple_t::vector(0, 1, 0) };
	const ray_t ray{ origin, direction };
	const matrix_t m{ matrix_t::scaling(2, 3, 4) };
	const tuple_t p{ tuple_t::point(2, 6, 12) };
	const tuple_t d{ tuple_t::vector(0, 3, 0) };
	const ray_t ray2{ ray.transform(m) };
	EXPECT_EQ(ray2.origin, p);
	EXPECT_EQ(ray2.direction, d);
}