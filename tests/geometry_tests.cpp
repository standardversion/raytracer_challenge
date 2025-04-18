﻿#include <cmath>
#include "gtest/gtest.h"
#include "../geometry.h"
#include "../phong.h"
#include "../intersection.h"
#include "../settings.h"

static ray_t test_helper_ray{ tuple_t::point(0, 0, 0), tuple_t::vector(0, 0, 1)};
// Test helper class since SceneObject consturctor is protected
class TestGeometry : public Geometry
{
public:
	TestGeometry()
		: Geometry{}
	{}
	tuple_t local_normal_at(const tuple_t& local_point) const
	{
		return tuple_t::vector(local_point.x, local_point.y, local_point.z);
	}

	void local_intersect(const ray_t& local_ray, intersections_t& intersections) const override
	{
		test_helper_ray = local_ray;
	}
};

/*
Scenario: The default material
  Given s ← TestGeometry()
  When m ← s.material
  Then m = Phong()
*/
TEST(geometry, should_have_default_material)
{
	const TestGeometry s{};
	std::shared_ptr<Phong> m = std::make_shared<Phong>();
	EXPECT_EQ(*s.material, *m);
}

/*
Scenario: Assigning a material
  Given s ← test_shape()
	And m ← material()
	And m.ambient ← 1
  When s.material ← m
  Then s.material = m
*/
TEST(geometry, should_be_able_to_assign_material)
{
	TestGeometry s{};
	std::shared_ptr<Phong> m = std::make_shared<Phong>();
	m->ambient = 1;
	s.material = m;
	EXPECT_EQ(*s.material, *m);
}

/*
Scenario: Intersecting a scaled shape with a ray
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
	And s ← test_shape()
  When s.transform = scaling(2, 2, 2)
	And xs ← s.intersect(r)
  Then test_helper_ray.origin = point(0, 0, -2.5)
	And test_helper_ray.direction = vector(0, 0, 0.5)
*/
TEST(geometry, should_be_able_to_intersect_scaled_object_with_a_ray)
{
	TestGeometry s{};
	s.transform = matrix_t::scaling(2, 2, 2);
	const ray_t r{ tuple_t::point(0, 0, -5), tuple_t::vector(0, 0, 1) };
	intersections_t i{};
	s.intersect(r, i);
	EXPECT_EQ(test_helper_ray.origin, tuple_t::point(0, 0, -2.5));
	EXPECT_EQ(test_helper_ray.direction, tuple_t::vector(0, 0, 0.5));
}

/*
Scenario: Intersecting a translated shape with a ray
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
	And s ← test_shape()
  When s.transform = translation(5, 0, 0)
	And xs ← s.intersect(r)
  Then test_helper_ray.origin = point(-5, 0, -5)
	And test_helper_ray.direction = vector(0, 0, 1)
*/
TEST(geometry, should_be_able_to_intersect_translated_object_with_a_ray)
{
	TestGeometry s{};
	s.transform = matrix_t::translation(5, 0, 0);
	const ray_t r{ tuple_t::point(0, 0, -5), tuple_t::vector(0, 0, 1) };
	intersections_t i{};
	s.intersect(r, i);
	EXPECT_EQ(test_helper_ray.origin, tuple_t::point(-5, 0, -5));
	EXPECT_EQ(test_helper_ray.direction, tuple_t::vector(0, 0, 1));
}

/*
Scenario: Computing the normal on a translated shape
  Given s ← test_shape()
  When set_transform(s, translation(0, 1, 0))
	And n ← s.normal_at(point(0, 1.70711, -0.70711))
  Then n = vector(0, 0.70711, -0.70711)
*/
TEST(geometry, should_be_able_to_compute_normal_on_a_translated_object)
{
	TestGeometry s{};
	s.transform = matrix_t::translation(0, 1, 0);
	const tuple_t n{ s.normal_at(tuple_t::point(0, 1.70711, -0.70711)) };
	EXPECT_EQ(n, tuple_t::vector(0, 0.70711, -0.70711));
}

/*
Scenario: Computing the normal on a transformed shape
  Given s ← test_shape()
	And m ← scaling(1, 0.5, 1) * rotation_z(π/5)
  When s.transform = m
	And n ← s.normal_at(point(0, √2/2, -√2/2))
  Then n = vector(0, 0.97014, -0.24254)
*/
TEST(geometry, should_be_able_to_compute_normal_on_a_transformed_object)
{
	TestGeometry s{};
	s.transform = matrix_t::scaling(1, 0.5, 1) * matrix_t::rotation_z(PI / 5);
	const tuple_t n{ s.normal_at(tuple_t::point(0, std::sqrt(2) / 2, -std::sqrt(2) / 2))};
	EXPECT_EQ(n, tuple_t::vector(0, 0.97014, -0.24254));
}