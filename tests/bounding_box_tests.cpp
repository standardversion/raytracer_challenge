#include "gtest/gtest.h"
#include "../tuple.h"
#include "../bounding_box.h"

/*
Scenario: Creating an empty bounding box
  Given box ← bounding_box(empty)
  Then box.min = point(infinity, infinity, infinity)
	And box.max = point(-infinity, -infinity, -infinity)
*/
TEST(bounding_box, should_create_empty_bounding_box)
{
	const bbox_t box{};
	EXPECT_EQ(box.min.x, INFINITY);
	EXPECT_EQ(box.min.y, INFINITY);
	EXPECT_EQ(box.min.z, INFINITY);
	EXPECT_EQ(box.max.x, -INFINITY);
	EXPECT_EQ(box.max.y, -INFINITY);
	EXPECT_EQ(box.max.z, -INFINITY);
}

/*
Scenario: Adding points to an empty bounding box
  Given box ← bounding_box(empty)
	And p1 ← point(-5, 2, 0)
	And p2 ← point(7, 0, -3)
  When p1 is added to box
	And p2 is added to box
  Then box.min = point(-5, 0, -3)
	And box.max = point(7, 2, 0)
*/
TEST(bounding_box, should_add_points_empty_bounding_box)
{
	bbox_t box{};
	const tuple_t p1{ tuple_t::point(-5, 2, 0) };
	const tuple_t p2{ tuple_t::point(7, 0, -3) };
	box.add(p1, p2);
	EXPECT_EQ(box.min, tuple_t::point(-5, 0, -3));
	EXPECT_EQ(box.max, tuple_t::point(7, 2, 0));
}