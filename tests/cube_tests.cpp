#include <vector>
#include "gtest/gtest.h"
#include "../cube.h"
#include "../ray.h"
#include "../intersection.h"
#include "../uv.h"

/*
Scenario Outline: A ray intersects a cube
  Given c ← cube()
    And r ← ray(<origin>, <direction>)
  When xs ← local_intersect(c, r)
  Then xs.count = 2
    And xs[0].t = <t1>
    And xs[1].t = <t2>

  Examples:
    |        | origin            | direction        | t1 | t2 |
    | +x     | point(5, 0.5, 0)  | vector(-1, 0, 0) |  4 |  6 |
    | -x     | point(-5, 0.5, 0) | vector(1, 0, 0)  |  4 |  6 |
    | +y     | point(0.5, 5, 0)  | vector(0, -1, 0) |  4 |  6 |
    | -y     | point(0.5, -5, 0) | vector(0, 1, 0)  |  4 |  6 |
    | +z     | point(0.5, 0, 5)  | vector(0, 0, -1) |  4 |  6 |
    | -z     | point(0.5, 0, -5) | vector(0, 0, 1)  |  4 |  6 |
    | inside | point(0, 0.5, 0)  | vector(0, 0, 1)  | -1 |  1 |
*/
TEST(cube, should_calculate_intersections_with_a_ray)
{
	const auto c{ Cube::create() };
    std::vector<ray_t> rays{
        { tuple_t::point(5, 0.5, 0), tuple_t::vector(-1, 0, 0) },
        { tuple_t::point(-5, 0.5, 0), tuple_t::vector(1, 0, 0) },
        { tuple_t::point(0.5, 5, 0), tuple_t::vector(0, -1, 0) },
        { tuple_t::point(0.5, -5, 0), tuple_t::vector(0, 1, 0) },
        { tuple_t::point(0.5, 0, 5), tuple_t::vector(0, 0, -1) },
        { tuple_t::point(0.5, 0, -5), tuple_t::vector(0, 0, 1) },
        { tuple_t::point(0, 0.5, 0), tuple_t::vector(0, 0, 1) }
    };
    int index{ 0 };
    intersections_t i{};
    for (const auto& r : rays)
    {
        c->intersect(r, i);
        EXPECT_EQ(i.entries.size(), 2);
        if (index == 6)
        {
            EXPECT_EQ(i[0].time, -1);
            EXPECT_EQ(i[1].time, 1);
        }
        else
        {
            EXPECT_EQ(i[0].time, 4);
            EXPECT_EQ(i[1].time, 6);
        }
        i.entries.clear();
        index++;
    }
}

/*
Scenario Outline: A ray misses a cube
  Given c ← cube()
    And r ← ray(<origin>, <direction>)
  When xs ← local_intersect(c, r)
  Then xs.count = 0

  Examples:
    | origin           | direction                      |
    | point(-2, 0, 0)  | vector(0.2673, 0.5345, 0.8018) |
    | point(0, -2, 0)  | vector(0.8018, 0.2673, 0.5345) |
    | point(0, 0, -2)  | vector(0.5345, 0.8018, 0.2673) |
    | point(2, 0, 2)   | vector(0, 0, -1)               |
    | point(0, 2, 2)   | vector(0, -1, 0)               |
    | point(2, 2, 0)   | vector(-1, 0, 0)               |
*/
TEST(cube, should_have_no_intersections_when_ray_misses_cube)
{
    const auto c{ Cube::create() };
    std::vector<ray_t> rays{
        { tuple_t::point(-2, 0, 0), tuple_t::vector(0.2673, 0.5345, 0.8018) },
        { tuple_t::point(0, -2, 0), tuple_t::vector(0.8018, 0.2673, 0.5345) },
        { tuple_t::point(0, 0, -2), tuple_t::vector(0.5345, 0.8018, 0.2673) },
        { tuple_t::point(2, 0, 2), tuple_t::vector(0, 0, -1) },
        { tuple_t::point(0, 2, 2), tuple_t::vector(0, -1, 0) },
        { tuple_t::point(2, 2, 0), tuple_t::vector(-1, 0, 0) }
    };
    int index{ 0 };
    intersections_t i{};
    for (const auto& r : rays)
    {
        c->intersect(r, i);
        EXPECT_EQ(i.entries.size(), 0);
        i.entries.clear();
        index++;
    }
}

/*
Scenario Outline: The normal on the surface of a cube
  Given c ← cube()
    And p ← <point>
  When normal ← local_normal_at(c, p)
  Then normal = <normal>

  Examples:
    | point                | normal           |
    | point(1, 0.5, -0.8)  | vector(1, 0, 0)  |
    | point(-1, -0.2, 0.9) | vector(-1, 0, 0) |
    | point(-0.4, 1, -0.1) | vector(0, 1, 0)  |
    | point(0.3, -1, -0.7) | vector(0, -1, 0) |
    | point(-0.6, 0.3, 1)  | vector(0, 0, 1)  |
    | point(0.4, 0.4, -1)  | vector(0, 0, -1) |
    | point(1, 1, 1)       | vector(1, 0, 0)  |
    | point(-1, -1, -1)    | vector(-1, 0, 0) |
*/
TEST(cube, should_calculate_normal_at_point)
{
    const auto c{ Cube::create() };
    std::vector<tuple_t> points{
        tuple_t::point(1, 0.5, -0.8),
        tuple_t::point(-1, -0.2, 0.9),
        tuple_t::point(-0.4, 1, -0.1),
        tuple_t::point(0.3, -1, -0.7),
        tuple_t::point(-0.6, 0.3, 1),
        tuple_t::point(0.4, 0.4, -1),
        tuple_t::point(1, 1, 1),
        tuple_t::point(-1, -1, -1)
    };
    std::vector<tuple_t> normals{
        tuple_t::vector(1, 0, 0),
        tuple_t::vector(-1, 0, 0),
        tuple_t::vector(0, 1, 0),
        tuple_t::vector(0, -1, 0),
        tuple_t::vector(0, 0, 1),
        tuple_t::vector(0, 0, -1),
        tuple_t::vector(1, 0, 0),
        tuple_t::vector(-1, 0, 0)
    };
    std::size_t index{ 0 };
    for (const auto& p : points)
    {
        const tuple_t n{c->local_normal_at(p)};
        EXPECT_EQ(n, normals[index]);
        index++;
    }
}

/*
Scenario: A cube has a bounding box
  Given shape ← cube()
  When box ← bounds_of(shape)
  Then box.min = point(-1, -1, -1)
    And box.max = point(1, 1, 1)
*/
TEST(cube, should_have_a_bounding_box)
{
    const auto c{ Cube::create() };
    const bbox_t box{ c->bounds() };
    EXPECT_EQ(box.min, tuple_t::point(-1, -1, -1));
    EXPECT_EQ(box.max, tuple_t::point(1, 1, 1));
}

/*
Scenario: Identifying the face of a cube from a point
  When face ← face_from_point(<point>)
  Then face = <face>

  Examples:
    | point                  | face    |
    | point(-1, 0.5, -0.25)  | "left"  |
    | point(1.1, -0.75, 0.8) | "right" |
    | point(0.1, 0.6, 0.9)   | "front" |
    | point(-0.7, 0, -2)     | "back"  |
    | point(0.5, 1, 0.9)     | "up"    |
    | point(-0.2, -1.3, 1.1) | "down"  |
*/
TEST(cube, should_get_correct_face_for_point)
{
    EXPECT_EQ(Cube::face_from_point(tuple_t::point(-1, 0.5, -0.25)), Cube_Faces::left);
    EXPECT_EQ(Cube::face_from_point(tuple_t::point(1.1, -0.75, 0.8)), Cube_Faces::right);
    EXPECT_EQ(Cube::face_from_point(tuple_t::point(0.1, 0.6, 0.9)), Cube_Faces::front);
    EXPECT_EQ(Cube::face_from_point(tuple_t::point(-0.7, 0, -2)), Cube_Faces::back);
    EXPECT_EQ(Cube::face_from_point(tuple_t::point(0.5, 1, 0.9)), Cube_Faces::up);
    EXPECT_EQ(Cube::face_from_point(tuple_t::point(-0.2, -1.3, 1.1)), Cube_Faces::down);
}

/*
Scenario: UV mapping the front face of a cube
  When (u, v) ← cube_uv_front(<point>)
  Then u = <u>
    And v = <v>

  Examples:
    | point                | u    | v    |
    | point(-0.5, 0.5, 1)  | 0.25 | 0.75 |
    | point(0.5, -0.5, 1)  | 0.75 | 0.25 |
*/
TEST(cube, should_return_uv_for_front_face_of_cube)
{
    const auto c{ Cube::create() };
    EXPECT_EQ(c->get_uv(tuple_t::point(-0.5, 0.5, 1)), uv_t(0.25, 0.75));
    EXPECT_EQ(c->get_uv(tuple_t::point(0.5, -0.5, 1)), uv_t(0.75, 0.25));
}

/*
Scenario: UV mapping the back face of a cube
  When (u, v) ← cube_uv_back(<point>)
  Then u = <u>
    And v = <v>

  Examples:
    | point                 | u    | v    |
    | point(0.5, 0.5, -1)   | 0.25 | 0.75 |
    | point(-0.5, -0.5, -1) | 0.75 | 0.25 |
*/
TEST(cube, should_return_uv_for_back_face_of_cube)
{
    const auto c{ Cube::create() };
    EXPECT_EQ(c->get_uv(tuple_t::point(0.5, 0.5, -1)), uv_t(0.25, 0.75));
    EXPECT_EQ(c->get_uv(tuple_t::point(-0.5, -0.5, -1)), uv_t(0.75, 0.25));
}

/*
Scenario: UV mapping the left face of a cube
  When (u, v) ← cube_uv_left(<point>)
  Then u = <u>
    And v = <v>

  Examples:
    | point                | u    | v    |
    | point(-1, 0.5, -0.5) | 0.25 | 0.75 |
    | point(-1, -0.5, 0.5) | 0.75 | 0.25 |
*/
TEST(cube, should_return_uv_for_left_face_of_cube)
{
    const auto c{ Cube::create() };
    EXPECT_EQ(c->get_uv(tuple_t::point(-1, 0.5, -0.5)), uv_t(0.25, 0.75));
    EXPECT_EQ(c->get_uv(tuple_t::point(-1, -0.5, 0.5)), uv_t(0.75, 0.25));
}

/*
Scenario: UV mapping the right face of a cube
  When (u, v) ← cube_uv_right(<point>)
  Then u = <u>
    And v = <v>

  Examples:
    | point                | u    | v    |
    | point(1, 0.5, 0.5)   | 0.25 | 0.75 |
    | point(1, -0.5, -0.5) | 0.75 | 0.25 |
*/
TEST(cube, should_return_uv_for_right_face_of_cube)
{
    const auto c{ Cube::create() };
    EXPECT_EQ(c->get_uv(tuple_t::point(1, 0.5, 0.5)), uv_t(0.25, 0.75));
    EXPECT_EQ(c->get_uv(tuple_t::point(1, -0.5, -0.5)), uv_t(0.75, 0.25));
}

/*
Scenario: UV mapping the upper face of a cube
  When (u, v) ← cube_uv_up(<point>)
  Then u = <u>
    And v = <v>

  Examples:
    | point                | u    | v    |
    | point(-0.5, 1, -0.5) | 0.25 | 0.75 |
    | point(0.5, 1, 0.5)   | 0.75 | 0.25 |
*/
TEST(cube, should_return_uv_for_upper_face_of_cube)
{
    const auto c{ Cube::create() };
    EXPECT_EQ(c->get_uv(tuple_t::point(-0.5, 1, -0.5)), uv_t(0.25, 0.75));
    EXPECT_EQ(c->get_uv(tuple_t::point(0.5, 1, 0.5)), uv_t(0.75, 0.25));
}

/*
Scenario: UV mapping the lower face of a cube
  When (u, v) ← cube_uv_down(<point>)
  Then u = <u>
    And v = <v>

  Examples:
    | point                 | u    | v    |
    | point(-0.5, -1, 0.5)  | 0.25 | 0.75 |
    | point(0.5, -1, -0.5)  | 0.75 | 0.25 |
*/
TEST(cube, should_return_uv_for_lower_face_of_cube)
{
    const auto c{ Cube::create() };
    EXPECT_EQ(c->get_uv(tuple_t::point(-0.5, -1, 0.5)), uv_t(0.25, 0.75));
    EXPECT_EQ(c->get_uv(tuple_t::point(0.5, -1, -0.5)), uv_t(0.75, 0.25));
}