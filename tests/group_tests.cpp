#include "gtest/gtest.h"
#include "../group.h"
#include "../matrix.h"
#include "../ray.h"
#include "../tuple.h"
#include "../intersection.h"
#include "../scene_object.h"
#include "../sphere.h"

class TestObject : public SceneObject
{
public:
	TestObject()
		: SceneObject{}
	{
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