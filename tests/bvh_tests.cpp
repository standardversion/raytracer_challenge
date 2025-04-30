#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "../bvh.h"
#include "../triangle.h"

void count_groups_recursive(const bvh_t& bvh, int& count) {
    count += 1;
    for (const auto& g : bvh.bvhs) {
        count_groups_recursive(*g, count);
    }
}

/*
  Scenario: Build with triangle count below threshold
    Given the BVH contains 3 triangles
    And the build threshold is 5
    When build is called
    Then the BVH should not change
    And no child BVH nodes should be created
*/
TEST(bvh, should_not_split_if_triangle_count_below_threshold)
{
    bvh_t bvh;
    for (int i = 0; i < 3; ++i) {
        bvh.add(Triangle::create({ float(i), 0, 0 }, { float(i + 0.5), 1, 0 }, { float(i + 1), 0, 0 }));
    }

    bvh.build(5);

    EXPECT_EQ(bvh.bvhs.size(), 0);
    EXPECT_EQ(bvh.triangles.size(), 3);
}

/*
  Scenario: Build with triangle count above threshold, but no split possible
    Given the BVH contains 6 overlapping triangles
    And the build threshold is 5
    When build is called
    Then the BVH should not change
    And no child BVH nodes should be created
*/
TEST(bvh, should_not_split_if_triangles_cannot_be_partitioned)
{
    bvh_t bvh;
    for (int i = 0; i < 6; ++i) {
        // All triangles at same position (overlapping bounds)
        bvh.add(Triangle::create({ 0, 0, 0 }, { 1, 0, 0 }, { 0, 1, 0 }));
    }

    bvh.build(5);

    EXPECT_EQ(bvh.bvhs.size(), 0);
    EXPECT_EQ(bvh.triangles.size(), 6);
}

/*
  Scenario: Build successfully splits triangles into two nodes
    Given the BVH contains 10 well-separated triangles
    And the build threshold is 5
    When build is called
    Then two child BVH nodes should be created
    And the root BVH node should have fewer triangles
*/
TEST(bvh, should_split_triangles_into_two_groups_if_possible)
{
    bvh_t bvh;
    for (int i = 0; i < 10; ++i) {
        float x = i * 2.0f;
        bvh.add(Triangle::create({ x, 0, 0 }, { x + 1, 1, 0 }, { x + 2, 0, 0 }));
    }

    bvh.build(5);

    EXPECT_EQ(bvh.bvhs.size(), 2);
    EXPECT_LT(bvh.triangles.size(), 10);  // Some triangles moved to children
}

/*
  Scenario: Build attempts to split but all triangles go to one side
    Given the BVH contains 6 triangles all within the left bounding box
    And the build threshold is 5
    When build is called
    Then the BVH should not change
    And no child BVH nodes should be created
*/
TEST(bvh, should_not_split_if_all_triangles_go_to_left_group)
{
    bvh_t bvh;
    for (int i = 0; i < 6; ++i) {
        float x = i * 0.01f; // tightly packed around origin
        bvh.add(Triangle::create({ x, 0, 0 }, { x + 0.001f, 1, 0 }, { x + 0.002f, 0, 0 }));
    }

    bvh.build(5);

    EXPECT_EQ(bvh.bvhs.size(), 0);
    EXPECT_EQ(bvh.triangles.size(), 6);
}

/*
  Scenario: Recursive build splits again if child exceeds threshold
    Given the BVH contains 20 well-separated triangles
    And the build threshold is 5
    When build is called
    Then at least two levels of child BVH nodes should exist
    And all triangle groups should respect the threshold
*/

TEST(bvh, should_split_recursively_if_child_exceeds_threshold)
{
    bvh_t bvh;
    for (int i = 0; i < 20; ++i) {
        float x = i * 3.0f;
        bvh.add(Triangle::create({ x, 0, 0 }, { x + 1, 1, 0 }, { x + 2, 0, 0 }));
    }

    bvh.build(5);

    int total_groups = 0;
    count_groups_recursive(bvh, total_groups);

    EXPECT_GT(total_groups, 2);
}

/*
  Scenario: Triangles not fitting into left or right box remain in root
    Given the BVH contains 6 triangles
    And 2 triangles do not fully fit into left or right bounding boxes
    And the build threshold is 5
    When build is called
    Then those 2 triangles should remain in the root BVH node
    And the rest should be in child BVH nodes

*/
TEST(bvh, triangles_not_fitting_left_or_right_stay_in_root)
{
    bvh_t bvh;

    // Two big triangles that straddle the mid-point
    bvh.add(Triangle::create({ -5, 0, 0 }, { 0, 10, 0 }, { 5, 0, 0 }));
    bvh.add(Triangle::create({ -6, 0, 0 }, { 0, 11, 0 }, { 6, 0, 0 }));

    // Four small triangles clearly on either side
    bvh.add(Triangle::create({ -10, 0, 0 }, { -9, 1, 0 }, { -8, 0, 0 }));
    bvh.add(Triangle::create({ -8, 0, 0 }, { -7, 1, 0 }, { -6, 0, 0 }));
    bvh.add(Triangle::create({ 8, 0, 0 }, { 9, 1, 0 }, { 10, 0, 0 }));
    bvh.add(Triangle::create({ 10, 0, 0 }, { 11, 1, 0 }, { 12, 0, 0 }));

    bvh.build(5);

    EXPECT_EQ(bvh.triangles.size(), 2);  // Big triangles stay in root
    EXPECT_EQ(bvh.bvhs.size(), 2); // Two children
}

/*
  Scenario: Adding a single triangle updates the BVH's bounding box
    Given an empty BVH
    When a triangle with bounds from (-1, -1, -1) to (1, 1, 1) is added
    Then the BVH should contain 1 triangle
    And the BVH's bounding box should span from (-1, -1, -1) to (1, 1, 1)
*/
TEST(bvh, should_update_bbox_when_adding_one_triangle)
{
    bvh_t bvh;

    auto t = Triangle::create(
        tuple_t::point(-1, -1, -1),
        tuple_t::point(1, -1, -1),
        tuple_t::point(0, 1, 1)
    );

    bvh.add(t);

    EXPECT_EQ(bvh.triangles.size(), 1);

    bbox_t expected = t->bounds();
    EXPECT_EQ(bvh.bbox.min, expected.min);
    EXPECT_EQ(bvh.bbox.max, expected.max);
}

/*
  Scenario: Adding multiple triangles expands the bounding box correctly
    Given an empty BVH
    When a triangle from (-1, -1, -1) to (1, 1, 1) is added
    And a triangle from (2, 2, 2) to (4, 4, 4) is added
    Then the BVH should contain 2 triangles
    And the BVH's bounding box should span from (-1, -1, -1) to (4, 4, 4)
*/
TEST(bvh, should_expand_bbox_when_adding_multiple_triangles)
{
    bvh_t bvh;

    auto t1 = Triangle::create(
        tuple_t::point(-1, -1, -1),
        tuple_t::point(1, -1, -1),
        tuple_t::point(0, 1, 1)
    );
    auto t2 = Triangle::create(
        tuple_t::point(2, 2, 2),
        tuple_t::point(3, 2, 2),
        tuple_t::point(4, 4, 4)
    );

    bvh.add(t1);
    bvh.add(t2);

    EXPECT_EQ(bvh.triangles.size(), 2);

    bbox_t expected;
    expected += t1->bounds();
    expected += t2->bounds();

    EXPECT_EQ(bvh.bbox.min, expected.min);
    EXPECT_EQ(bvh.bbox.max, expected.max);
}

/*
Scenario: Ray misses the BVH bounding box
  Given a BVH with one triangle
  And a ray that does not intersect the BVH's bounding box
  When local_intersect is called
  Then there should be no intersections
*/
TEST(bvh, should_not_intersect_if_ray_misses_bbox)
{
    bvh_t bvh;

    auto tri = Triangle::create(
        tuple_t::point(0, 0, 0),
        tuple_t::point(1, 0, 0),
        tuple_t::point(0, 1, 0)
    );

    bvh.add(tri);

    ray_t ray{ tuple_t::point(10, 10, -5), tuple_t::vector(0, 0, 1) };
    intersections_t intersections;

    bvh.local_intersect(ray, intersections);

    EXPECT_TRUE(intersections.entries.empty());
}

/*
Scenario: Ray hits the BVH bounding box and a triangle inside
  Given a BVH with one triangle
  And a ray that intersects the BVH's bounding box and the triangle
  When local_intersect is called
  Then there should be one intersection
*/
TEST(bvh, should_intersect_triangle_if_ray_hits_bbox_and_triangle)
{
    bvh_t bvh;

    auto tri = Triangle::create(
        tuple_t::point(0, 0, 0),
        tuple_t::point(1, 0, 0),
        tuple_t::point(0, 1, 0)
    );

    bvh.add(tri);

    ray_t ray{ tuple_t::point(0.25, 0.25, -1), tuple_t::vector(0, 0, 1) };
    intersections_t intersections;

    bvh.local_intersect(ray, intersections);

    EXPECT_EQ(intersections.entries.size(), 1);
}
