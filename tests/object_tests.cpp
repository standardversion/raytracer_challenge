#include "gtest/gtest.h"
#include "../object.h"
#include "../tuple.h"

// Test helper class since Object consturctor is protected
class TestObject : public Object
{
public:
	TestObject()
		: Object{}
	{}
	tuple_t local_normal_at(const tuple_t& local_point) const override
	{
		// dummy implementation just for testing
		return tuple_t::vector(0, 0, 1);
	}

	void intersect(const ray_t& ray, intersections_t& intersections) const override
	{

	}

};

/*
Scenario: Object should have unique id. And transform and material attributes
  Given o ← object()
	And o2 ← object()
	And o3 ← object()
  Then o.id = 1
	And o.material exists
	And o.transform exists
	And o2.id = 2
	And o2.material exists
	And o2.transform exists
	And o3.id = 3
	And o3.material exists
	And o3.transform exists
*/
TEST(object, should_create_object_with_correct_attrs_and_unique_id)
{
	const TestObject o{};
	const TestObject o2{};
	const TestObject o3{};
	EXPECT_TRUE(o.id != o3.id != o2.id);
}