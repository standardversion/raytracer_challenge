#include "gtest/gtest.h"
#include "../tuple.h"
#include "../bounding_box.h"
#include "../matrix.h"
#include "../settings.h"

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

/*
Scenario: Adding one bounding box to another
  Given box1 ← bounding_box(min=point(-5, -2, 0) max=point(7, 4, 4))
	And box2 ← bounding_box(min=point(8, -7, -2) max=point(14, 2, 8))
  When box2 is added to box1
  Then box1.min = point(-5, -7, -2)
	And box1.max = point(14, 4, 8)
*/
TEST(bounding_box, should_be_able_to_add_a_bounding_box_to_another)
{
	bbox_t box1{ tuple_t::point(-5, -2, 0), tuple_t::point(7, 4, 4) };
	bbox_t box2{ tuple_t::point(8, -7, -2), tuple_t::point(14, 2, 8) };
	box1 += box2;
	EXPECT_EQ(box1.min, tuple_t::point(-5, -7, -2));
	EXPECT_EQ(box1.max, tuple_t::point(14, 4, 8));
}

/*
Scenario Outline: Checking to see if a box contains a given point
  Given box ← bounding_box(min=point(5, -2, 0) max=point(11, 4, 7))
	And p ← <point>
  Then box_contains_point(box, p) is <result>

  Examples:
	| point           | result |
	| point(5, -2, 0) | true   |
	| point(11, 4, 7) | true   |
	| point(8, 1, 3)  | true   |
	| point(3, 0, 3)  | false  |
	| point(8, -4, 3) | false  |
	| point(8, 1, -1) | false  |
	| point(13, 1, 3) | false  |
	| point(8, 5, 3)  | false  |
	| point(8, 1, 8)  | false  |
*/
TEST(bounding_box, should_be_able_to_check_if_bbox_contains_a_point)
{
	const bbox_t box{ tuple_t::point(5, -2, 0), tuple_t::point(11, 4, 7) };
	EXPECT_TRUE(box.contains(tuple_t::point(5, -2, 0)));
	EXPECT_TRUE(box.contains(tuple_t::point(11, 4, 7)));
	EXPECT_TRUE(box.contains(tuple_t::point(8, 1, 3)));
	EXPECT_FALSE(box.contains(tuple_t::point(3, 0, 3)));
	EXPECT_FALSE(box.contains(tuple_t::point(8, -4, 3)));
	EXPECT_FALSE(box.contains(tuple_t::point(8, 1, -1)));
	EXPECT_FALSE(box.contains(tuple_t::point(13, 1, 3)));
	EXPECT_FALSE(box.contains(tuple_t::point(8, 5, 3)));
	EXPECT_FALSE(box.contains(tuple_t::point(8, 1, 8)));
}

/*
Scenario Outline: Checking to see if a box contains a given box
  Given box ← bounding_box(min=point(5, -2, 0) max=point(11, 4, 7))
	And box2 ← bounding_box(min=<min> max=<max>)
  Then box_contains_box(box, box2) is <result>

  Examples:
	| min              | max             | result |
	| point(5, -2, 0)  | point(11, 4, 7) | true   |
	| point(6, -1, 1)  | point(10, 3, 6) | true   |
	| point(4, -3, -1) | point(10, 3, 6) | false  |
	| point(6, -1, 1)  | point(12, 5, 8) | false  |
*/
TEST(bounding_box, should_be_able_to_check_if_bbox_contains_another_box)
{
	const bbox_t box{ tuple_t::point(5, -2, 0), tuple_t::point(11, 4, 7) };
	EXPECT_TRUE(box.contains(bbox_t{ tuple_t::point(5, -2, 0), tuple_t::point(11, 4, 7) }));
	EXPECT_TRUE(box.contains(bbox_t{ tuple_t::point(6, -1, 1), tuple_t::point(10, 3, 6) }));
	EXPECT_FALSE(box.contains(bbox_t{ tuple_t::point(4, -3, -1), tuple_t::point(10, 3, 6) }));
	EXPECT_FALSE(box.contains(bbox_t{ tuple_t::point(6, -1, 1), tuple_t::point(12, 5, 8) }));
}

/*
Scenario: Transforming a bounding box
  Given box ← bounding_box(min=point(-1, -1, -1) max=point(1, 1, 1))
	And matrix ← rotation_x(π / 4) * rotation_y(π / 4)
  When box2 ← transform(box, matrix)
  Then box2.min = point(-1.4142, -1.7071, -1.7071)
	And box2.max = point(1.4142, 1.7071, 1.7071)
*/
TEST(bounding_box, should_be_able_to_transform_bbox)
{
	const bbox_t box{ tuple_t::point(-1, -1, -1), tuple_t::point(1, 1, 1) };
	const matrix_t transform{ matrix_t::rotation_x(PI / 4) * matrix_t::rotation_y(PI / 4) };
	const bbox_t box2{ box.transform(transform) };
	EXPECT_EQ(box2.min, tuple_t::point(-1.4142, -1.7071, -1.7071));
	EXPECT_EQ(box2.max, tuple_t::point(1.4142, 1.7071, 1.7071));
}