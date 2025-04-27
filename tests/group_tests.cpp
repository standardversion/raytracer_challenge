#include "gtest/gtest.h"
#include "../group.h"
#include "../matrix.h"
#include "../ray.h"
#include "../tuple.h"
#include "../intersection.h"
#include "../scene_object.h"
#include "../sphere.h"
#include "../bounding_box.h"
#include "../cylinder.h"
#include "../geometry.h"

class TestObject : public SceneObject
{
public:
	TestObject()
		: SceneObject{}
	{
	}

};

static ray_t test_helper_ray{ tuple_t::point(0, 0, 0), tuple_t::vector(0, 0, 0) };
// Test helper class since SceneObject consturctor is protected
class TestGrpGeometry : public Geometry
{
public:
	TestGrpGeometry()
		: Geometry{}
	{
	}
	tuple_t local_normal_at(const tuple_t& local_point, const double alpha, const double beta, const double gamma) const
	{
		return tuple_t::vector(local_point.x, local_point.y, local_point.z);
	}

	void local_intersect(const ray_t& local_ray, intersections_t& intersections) const override
	{
		if (local_ray.origin == tuple_t::point(0, 0, -5) && local_ray.direction == tuple_t::vector(0, 0, 1))
		{
			test_helper_ray = local_ray;
		}
	}
	bbox_t bounds() const override
	{
		return bbox_t{ tuple_t::point(-1, -1, -1), tuple_t::point(1, 1, 1) };
	}
};

/*
Scenario: Creating a new group
  Given g ← group()
  Then g.transform = identity_matrix
	And g is empty
*/
TEST(group, should_be_able_to_create_new_group)
{
	Group g{};
	EXPECT_EQ(g.transform, matrix_t::identity());
	EXPECT_TRUE(g.children.empty());
}

/*
Scenario: Adding a child to a group
  Given g ← group()
	And s ← test_shape()
  When add_child(g, s)
  Then g is not empty
	And g includes s
	And s.parent = g
*/
TEST(group, should_be_able_to_add_child_to_group)
{
	auto g{ std::make_shared<Group>() };
	auto t{ std::make_shared<TestObject>() };
	g->add(t);
	auto found_t{ g->find(t.get()) };
	EXPECT_EQ(g->transform, matrix_t::identity());
	EXPECT_EQ(g->find(t.get()).get(), t.get());
	EXPECT_EQ(found_t->parent.lock().get(), g.get());
	EXPECT_FALSE(g->children.empty());
}

/*
Scenario: Intersecting a ray with an empty group
  Given g ← group()
	And r ← ray(point(0, 0, 0), vector(0, 0, 1))
  When xs ← local_intersect(g, r)
  Then xs is empty
*/
TEST(group, should_have_no_intersections_when_group_is_empty)
{
	Group g{};
	const ray_t r{ tuple_t::point(0, 0, 0), tuple_t::vector(0, 0, 1) };
	intersections_t i{};
	g.intersect(r, i);
	EXPECT_TRUE(i.entries.empty());
}

/*
Scenario: Intersecting a ray with a nonempty group
  Given g ← group()
	And s1 ← sphere()
	And s2 ← sphere()
	And set_transform(s2, translation(0, 0, -3))
	And s3 ← sphere()
	And set_transform(s3, translation(5, 0, 0))
	And add_child(g, s1)
	And add_child(g, s2)
	And add_child(g, s3)
  When r ← ray(point(0, 0, -5), vector(0, 0, 1))
	And xs ← local_intersect(g, r)
  Then xs.count = 4
	And xs[0].object = s2
	And xs[1].object = s2
	And xs[2].object = s1
	And xs[3].object = s1
*/
TEST(group, should_have_intersections_when_group_is_not_empty)
{
	auto g{ std::make_shared<Group>() };
	auto s1{ Sphere::create() };
	auto s2{ Sphere::create() };
	auto s3{ Sphere::create() };
	g->add(s1, s2, s3);
	s2->transform = matrix_t::translation(0, 0, -3);
	s3->transform = matrix_t::translation(5, 0, 0);
	const ray_t r{ tuple_t::point(0, 0, -5), tuple_t::vector(0, 0, 1) };
	intersections_t i{};
	g->intersect(r, i);
	EXPECT_EQ(i.entries.size(), 4);
	EXPECT_EQ(i[0].object.get(), s2.get());
	EXPECT_EQ(i[1].object.get(), s2.get());
	EXPECT_EQ(i[2].object.get(), s1.get());
	EXPECT_EQ(i[3].object.get(), s1.get());
}

/*
Scenario: Intersecting a transformed group
  Given g ← group()
	And set_transform(g, scaling(2, 2, 2))
	And s ← sphere()
	And set_transform(s, translation(5, 0, 0))
	And add_child(g, s)
  When r ← ray(point(10, 0, -10), vector(0, 0, 1))
	And xs ← intersect(g, r)
  Then xs.count = 2
*/
TEST(group, should_have_intersections_when_group_is_transformed)
{
	auto g{ std::make_shared<Group>() };
	g->transform = matrix_t::scaling(2, 2, 2);
	auto s{ Sphere::create() };
	g->add(s);
	s->transform = matrix_t::translation(5, 0, 0);
	const ray_t r{ tuple_t::point(10, 0, -10), tuple_t::vector(0, 0, 1) };
	intersections_t i{};
	g->intersect(r, i);
	EXPECT_EQ(i.entries.size(), 2);
}

/*
Scenario: A group has a bounding box that contains its children
  Given s ← sphere()
	And set_transform(s, translation(2, 5, -3) * scaling(2, 2, 2))
	And c ← cylinder()
	And c.minimum ← -2
	And c.maximum ← 2
	And set_transform(c, translation(-4, -1, 4) * scaling(0.5, 1, 0.5))
	And shape ← group()
	And add_child(shape, s)
	And add_child(shape, c)
  When box ← bounds_of(shape)
  Then box.min = point(-4.5, -3, -5)
	And box.max = point(4, 7, 4.5)
*/
TEST(group, should_have_bounding_box_containing_its_children)
{
	auto g{ std::make_shared<Group>() };
	auto s{ Sphere::create() };
	g->add(s);
	s->transform = matrix_t::translation(2, 5, -3) * matrix_t::scaling(2, 2, 2);
	auto c{ Cylinder::create() };
	c->minimum = -2;
	c->maximum = 2;
	c->transform = matrix_t::translation(-4, -1, 4) * matrix_t::scaling(0.5, 1, 0.5);
	g->add(c);
	const bbox_t box{ g->bounds() };
	EXPECT_EQ(box.min, tuple_t::point(-4.5, -3, -5));
	EXPECT_EQ(box.max, tuple_t::point(4, 7, 4.5));
}

/*
Scenario: Intersecting ray+group doesn't test children if box is missed
  Given child ← test_shape()
	And shape ← group()
	And add_child(shape, child)
	And r ← ray(point(0, 0, -5), vector(0, 1, 0))
  When xs ← intersect(shape, r)
  Then child.saved_ray is unset
*/
TEST(group, should_not_test_children_if_box_is_missed)
{
	auto g{ std::make_shared<Group>() };
	auto t{ std::make_shared<TestGrpGeometry>() };
	g->add(t);
	const ray_t r{ tuple_t::point(0, 0, - 5), tuple_t::vector(0, 1, 0) };
	intersections_t i{};
	g->intersect(r, i);
	EXPECT_EQ(test_helper_ray.origin, tuple_t::point(0, 0, 0));
	EXPECT_EQ(test_helper_ray.direction, tuple_t::vector(0, 0, 0));
}

/*
Scenario: Intersecting ray+group tests children if box is hit
  Given child ← test_shape()
	And shape ← group()
	And add_child(shape, child)
	And r ← ray(point(0, 0, -5), vector(0, 0, 1))
  When xs ← intersect(shape, r)
  Then child.saved_ray is set
*/
TEST(group, should_test_children_if_box_is_hit)
{
	auto g{ std::make_shared<Group>() };
	auto t{ std::make_shared<TestGrpGeometry>() };
	g->add(t);
	const ray_t r{ tuple_t::point(0, 0, -5), tuple_t::vector(0, 0, 1) };
	intersections_t i{};
	g->intersect(r, i);
	EXPECT_EQ(test_helper_ray.origin, tuple_t::point(0, 0, -5));
	EXPECT_EQ(test_helper_ray.direction, tuple_t::vector(0, 0, 1));
}