#include "gtest/gtest.h"
#include "../sphere.h"
#include "../ray.h"
#include "../intersection.h"
#include "../matrix.h"
#include "../settings.h"
#include "../phong.h"

/*
Scenario: Creating and querying a sphere
  Given s ← sphere(1)
	And s2 ← sphere(5)
	And s3 ← sphere(4)
  Then s->radius = 1
	And s2.radius = 5
	And s3.radius = 4
	And s1.id != s2.id != s3.id
*/
TEST(sphere, should_create_spheres_with_unique_ids)
{
	const auto s{ Sphere::create(1) };
	const auto s2{ Sphere::create(5) };
	const auto s3{ Sphere::create(4) };
	EXPECT_EQ(s->radius, 1);
	EXPECT_EQ(s2->radius, 5);
	EXPECT_EQ(s3->radius, 4);
	EXPECT_TRUE(s->id != s3->id != s2->id);
}

/*
Scenario: A ray intersects a sphere at two points
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
	And s ← sphere()
	And xs ← intersections()
  When xs ← s->intersect(r, intersections)
  Then xs.entries.size() = 2
	And xs[0].time = 4.0
	And xs[1].time = 6.0
*/
TEST(sphere, should_intersect_sphere_at_2_points)
{
	const tuple_t origin{ tuple_t::point(0, 0, -5) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const ray_t r{ origin, direction };
	const auto s{ Sphere::create() };
	intersections_t intersections{};
	s->intersect(r, intersections);
	EXPECT_EQ(intersections.entries.size(), 2);
	EXPECT_EQ(intersections[0].time, 4.0);
	EXPECT_EQ(intersections[1].time, 6.0);
}

/*
Scenario: A ray intersects a sphere at a tangent
  Given r ← ray(point(0, 1, -5), vector(0, 0, 1))
	And s ← sphere()
  And xs ← intersections()
  When xs ← s->intersect(r, intersections)
  Then xs.entries.size() = 2
	And xs[0].time = 5.0
	And xs[1].time = 5.0
*/
TEST(sphere, should_intersect_sphere_at_tangent)
{
	const tuple_t origin{ tuple_t::point(0, 1, -5) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const ray_t r{ origin, direction };
	const auto s{ Sphere::create() };
	intersections_t intersections{};
	s->intersect(r, intersections);
	EXPECT_EQ(intersections.entries.size(), 2);
	EXPECT_EQ(intersections[0].time, 5.0);
	EXPECT_EQ(intersections[1].time, 5.0);
}

/*
Scenario: A ray misses a sphere
  Given r ← ray(point(0, 2, -5), vector(0, 0, 1))
	And s ← sphere()
  And xs ← intersections()
  When xs ← s->intersect(r, intersections)
  Then xs.entries.size() = 0
*/
TEST(sphere, should_miss_sphere)
{
	const tuple_t origin{ tuple_t::point(0, 2, -5) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const ray_t r{ origin, direction };
	const auto s{ Sphere::create() };
	intersections_t intersections{};
	s->intersect(r, intersections);
	EXPECT_EQ(intersections.entries.size(), 0);
}

/*
Scenario: A ray originates inside a sphere
  Given r ← ray(point(0, 0, 0), vector(0, 0, 1))
	And s ← sphere()
  And xs ← intersections()
  When xs ← s->intersect(r, intersections)
  Then xs.entries.size() = 2
	And xs[0].time = -1.0
	And xs[1].time = 1.0
*/
TEST(sphere, should_intersect_sphere_even_if_ray_originates_inside_sphere)
{
	const tuple_t origin{ tuple_t::point(0, 0, 0) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const ray_t r{ origin, direction };
	const auto s{ Sphere::create() };
	intersections_t intersections{};
	s->intersect(r, intersections);
	EXPECT_EQ(intersections.entries.size(), 2);
	EXPECT_EQ(intersections[0].time, -1.0);
	EXPECT_EQ(intersections[1].time, 1.0);
}

/*
Scenario: A sphere is behind a ray
  Given r ← ray(point(0, 0, 5), vector(0, 0, 1))
	And s ← sphere()
  And xs ← intersections()
  When xs ← s->intersect(r, intersections)
  Then xs.entries.size() = 2
	And xs[0].time = -6.0
	And xs[1].time = -4.0
*/
TEST(sphere, should_intersect_sphere_even_if_ray_is_behind_sphere)
{
	const tuple_t origin{ tuple_t::point(0, 0, 5) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const ray_t r{ origin, direction };
	const auto s{ Sphere::create() };
	intersections_t intersections{};
	s->intersect(r, intersections);
	EXPECT_EQ(intersections.entries.size(), 2);
	EXPECT_EQ(intersections[0].time, -6.0);
	EXPECT_EQ(intersections[1].time, -4.0);
}

/*
Scenario: A sphere's default transformation
  Given s ← sphere()
  Then s->transform = identity_matrix
*/
TEST(sphere, should_have_default_transform_for_sphere)
{
	const tuple_t origin{ tuple_t::point(0, 0, 5) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const matrix_t i{ matrix_t::identity() };
	const auto s{ Sphere::create() };
	EXPECT_EQ(s->transform, i);
}

/*
Scenario: Changing a sphere's transformation
  Given s ← sphere()
	And t ← translation(2, 3, 4)
  When s->transform = t
  Then s->transform = t
*/
TEST(sphere, should_be_able_to_set_spheres_transform)
{
	const tuple_t origin{ tuple_t::point(0, 0, 5) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const matrix_t t{ matrix_t::translation(2, 3, 4) };
	const auto s{ Sphere::create() };
	s->transform = t;
	EXPECT_EQ(s->transform, t);
}

/*
Scenario: Intersecting a scaled sphere with a ray
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
	And s ← sphere()
  When s->transform = scaling(2, 2, 2)
	And xs ← s->intersect(r)
  Then xs.count = 2
	And xs[0].t = 3
	And xs[1].t = 7
*/
TEST(sphere, should_intersect_scaled_sphere)
{
	const tuple_t origin{ tuple_t::point(0, 0, -5) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const ray_t r{ origin, direction };
	const auto s{ Sphere::create() };
	const matrix_t t{ matrix_t::scaling(2, 2, 2) };
	s->transform = t;
	intersections_t intersections{};
	s->intersect(r, intersections);
	EXPECT_EQ(intersections.entries.size(), 2);
	EXPECT_EQ(intersections[0].time, 3);
	EXPECT_EQ(intersections[1].time, 7);
}

/*
Scenario: Intersecting a translated sphere with a ray
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
	And s ← sphere()
  When set_transform(s, translation(5, 0, 0))
	And xs ← s->intersect(r)
  Then xs.count = 0
*/
TEST(sphere, should_intersect_translated_sphere)
{
	const tuple_t origin{ tuple_t::point(0, 0, -5) };
	const tuple_t direction{ tuple_t::vector(0, 0, 1) };
	const ray_t r{ origin, direction };
	const auto s{ Sphere::create() };
	const matrix_t t{ matrix_t::translation(5, 0, 0) };
	s->transform = t;
	intersections_t intersections{};
	s->intersect(r, intersections);
	EXPECT_EQ(intersections.entries.size(), 0);
}

/*
Scenario: The normal on a sphere at a point on the x axis
  Given s ← sphere()
  When n ← s->normal_at(point(1, 0, 0))
  Then n = vector(1, 0, 0)
*/
TEST(sphere, should_return_the_normal_on_a_sphere_at_a_point_on_the_x_axis)
{
	const auto s{ Sphere::create() };
	const tuple_t r{ tuple_t::vector(1, 0, 0) };
	const tuple_t n{ s->normal_at(tuple_t::point(1, 0, 0)) };
	EXPECT_EQ(n, r);
}

/*
Scenario: The normal on a sphere at a point on the y axis
  Given s ← sphere()
  When n ← s->normal_at(point(0, 1, 0))
  Then n = vector(0, 1, 0)
*/
TEST(sphere, should_return_the_normal_on_a_sphere_at_a_point_on_the_y_axis)
{
	const auto s{ Sphere::create() };
	const tuple_t r{ tuple_t::vector(0, 1, 0) };
	const tuple_t n{ s->normal_at(tuple_t::point(0, 1, 0)) };
	EXPECT_EQ(n, r);
}

/*
Scenario: The normal on a sphere at a point on the z axis
  Given s ← sphere()
  When n ← s->normal_at(point(0, 0, 1))
  Then n = vector(0, 0, 1)
*/
TEST(sphere, should_return_the_normal_on_a_sphere_at_a_point_on_the_z_axis)
{
	const auto s{ Sphere::create() };
	const tuple_t r{ tuple_t::vector(0, 0, 1) };
	const tuple_t n{ s->normal_at(tuple_t::point(0, 0, 1)) };
	EXPECT_EQ(n, r);
}

/*
Scenario: The normal on a sphere at a nonaxial point
  Given s ← sphere()
  When n ← s->normal_at(point(√3/3, √3/3, √3/3))
  Then n = vector(√3/3, √3/3, √3/3)
*/
TEST(sphere, should_return_the_normal_on_a_sphere_at_a_nonaxial_point)
{
	const auto s{ Sphere::create() };
	const tuple_t r{ tuple_t::vector(std::sqrt(3) /3, std::sqrt(3) / 3, std::sqrt(3) / 3)};
	const tuple_t n{ s->normal_at(tuple_t::point(std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3)) };
	EXPECT_EQ(n, r);
}

/*
Scenario: The normal is a normalized vector
  Given s ← sphere()
  When n ← s->normal_at(point(√3/3, √3/3, √3/3))
  Then n = normalize(n)
*/
TEST(sphere, should_return_the_normalized_normal)
{
	const auto s{ Sphere::create() };
	tuple_t r{ tuple_t::vector(std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3) };
	const tuple_t n{ s->normal_at(tuple_t::point(std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3)) };
	r.normalize();
	EXPECT_EQ(n, r);
}

/*
Scenario: Computing the normal on a translated sphere
  Given s ← sphere()
	And s->transform = translation(0, 1, 0)
  When n ← s->normal_at(point(0, 1.70711, -0.70711))
  Then n = vector(0, 0.70711, -0.70711)
*/
TEST(sphere, should_compute_normal_on_a_translated_sphere)
{
	const auto s{ Sphere::create() };
	s->transform = matrix_t::translation(0, 1, 0);
	tuple_t r{ tuple_t::vector(0, 0.70711, -0.70711) };
	const tuple_t n{ s->normal_at(tuple_t::point(0, 1.70711, -0.70711)) };
	r.normalize();
	EXPECT_EQ(n, r);
}

/*
Scenario: Computing the normal on a transformed sphere
  Given s ← sphere()
	And m ← scaling(1, 0.5, 1) * rotation_z(π/5)
	And s->transform = m
  When n ← s->normal_at(point(0, √2/2, -√2/2))
  Then n = vector(0, 0.97014, -0.24254)

*/
TEST(sphere, should_compute_normal_on_a_transformed_sphere)
{
	const auto s{ Sphere::create() };
	s->transform = matrix_t::scaling(1, 0.5, 1) * matrix_t::rotation_z(PI / 5);
	tuple_t r{ tuple_t::vector(0, 0.97014, -0.24254) };
	const tuple_t n{ s->normal_at(tuple_t::point(0, std::sqrt(2) / 2, -std::sqrt(2) / 2)) };
	EXPECT_EQ(n, r);
}

/*
Scenario: A sphere has a default material
  Given s ← sphere()
  When m ← s->material
  Then m = material()
*/
TEST(sphere, should_has_a_default_material)
{
	const auto s{ Sphere::create() };
	std::shared_ptr<Phong> m = std::make_shared<Phong>();
	EXPECT_EQ(*s->material, *m);
}

/*
Scenario: A sphere may be assigned a material
  Given s ← sphere()
	And m ← material()
	And m.ambient ← 1
  When s->material ← m
  Then s->material = m
*/
TEST(sphere, should_be_able_to_assign_material)
{
	const auto s{ Sphere::create() };
	std::shared_ptr<Phong> m{ std::make_shared<Phong>() };
	m->ambient = 1;
	s->material = m;
	EXPECT_EQ(s->material.get(), m.get());
}