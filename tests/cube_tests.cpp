#include <vector>
#include "gtest/gtest.h"
#include "../cube.h"
#include "../ray.h"
#include "../intersection.h"

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