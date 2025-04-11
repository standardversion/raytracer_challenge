#include "gtest/gtest.h"
#include "../tuple.h"
#include "../ray.h"

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