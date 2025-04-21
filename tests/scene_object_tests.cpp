#include <cmath>
#include "gtest/gtest.h"
#include "../scene_object.h"
#include "../matrix.h"
#include "../group.h"
#include "../settings.h"
#include "../sphere.h"

// Test helper class since SceneObject consturctor is protected
class TestObject : public SceneObject
{
public:
	TestObject()
		: SceneObject{}
	{}

};

/*
Scenario: Object should have unique id. And transform and material attributes
  Given o ← TestObject()
	And o2 ← TestObject()
	And o3 ← TestObject()
  Then o.id != o2.id != o3.id
*/
TEST(scene_object, should_create_object_with_unique_id)
{
	const TestObject o{};
	const TestObject o2{};
	const TestObject o3{};
	EXPECT_TRUE(o.id != o3.id != o2.id);
}

/*
Scenario: The default transformation
  Given s ← TestObject()
  Then s.transform = identity_matrix
*/
TEST(scene_object, should_create_object_with_default_transformation)
{
	const TestObject o{};
	EXPECT_EQ(o.transform, matrix_t::identity());
}

/*
Scenario: Assigning a transformation
  Given s ← TestObject()
  When s.transform = translation(2, 3, 4)
  Then s.transform = translation(2, 3, 4)
*/
TEST(scene_object, should_be_able_to_set_transform)
{
	TestObject o{};
	const matrix_t t{ matrix_t::translation(2, 3, 4) };
	o.transform = t;
	EXPECT_EQ(o.transform, t);
}

/*
Scenario: Converting a point from world to object space
  Given g1 ← group()
	And set_transform(g1, rotation_y(π/2))
	And g2 ← group()
	And set_transform(g2, scaling(2, 2, 2))
	And add_child(g1, g2)
	And s ← sphere()
	And set_transform(s, translation(5, 0, 0))
	And add_child(g2, s)
  When p ← world_to_object(s, point(-2, 0, -10))
  Then p = point(0, 0, -1)
*/
TEST(scene_object, should_be_able_to_covert_from_world_to_object_space)
{
	Group g1{};
	g1.transform = matrix_t::rotation_y(PI / 2);
	Group g2{};
	g2.transform = matrix_t::scaling(2, 2, 2);
	g1.add(&g2);
	auto s{ Sphere::create() };
	s->transform = matrix_t::translation(5, 0, 0);
	g2.add(s.get());
	EXPECT_EQ(s->world_to_object(tuple_t::point(-2, 0, -10)), tuple_t::point(0, 0, -1));
}

/*
Scenario: Converting a normal from object to world space
  Given g1 ← group()
	And set_transform(g1, rotation_y(π/2))
	And g2 ← group()
	And set_transform(g2, scaling(1, 2, 3))
	And add_child(g1, g2)
	And s ← sphere()
	And set_transform(s, translation(5, 0, 0))
	And add_child(g2, s)
  When n ← normal_to_world(s, vector(√3/3, √3/3, √3/3))
  Then n = vector(0.2857, 0.4286, -0.8571)
*/
TEST(scene_object, should_be_able_to_covert_normal_from_object__to_world_space)
{
	Group g1{};
	g1.transform = matrix_t::rotation_y(PI / 2);
	Group g2{};
	g2.transform = matrix_t::scaling(1, 2, 3);
	g1.add(&g2);
	auto s{ Sphere::create() };
	s->transform = matrix_t::translation(5, 0, 0);
	g2.add(s.get());
	EXPECT_EQ(
		s->normal_to_world(tuple_t::vector(std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3)),
		tuple_t::vector(0.2857, 0.4286, -0.8571)
	);
}