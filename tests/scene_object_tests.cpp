#include "gtest/gtest.h"
#include "../scene_object.h"
#include "../tuple.h"

// Test helper class since Object consturctor is protected
class TestObject : public SceneObject
{
public:
	TestObject()
		: SceneObject{}
	{}

};

/*
Scenario: Object should have unique id. And transform and material attributes
  Given o ← object()
	And o2 ← object()
	And o3 ← object()
  Then o.id != o2.id != o3.id
*/
TEST(object, should_create_object_with_correct_attrs_and_unique_id)
{
	const TestObject o{};
	const TestObject o2{};
	const TestObject o3{};
	EXPECT_TRUE(o.id != o3.id != o2.id);
}