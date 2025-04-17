#include "gtest/gtest.h"
#include "../scene_object.h"
#include "../matrix.h"

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