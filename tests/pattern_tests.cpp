#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "../colour.h"
#include "../pattern.h"
#include "../tuple.h"
#include "../sphere.h"
#include "../matrix.h"

// Test helper class since Pattern consturctor is protected
class TestPattern : public Pattern
{
public:
	TestPattern(const colour_t& a, const colour_t& b)
		: Pattern{ a, b }
	{
	}
	colour_t at(const tuple_t& point) const
	{
		return { point.x, point.y, point.z };
	}
};

/*
Scenario: The default pattern transformation
  Given pattern ← test_pattern()
  Then pattern.transform = identity_matrix
*/
TEST(pattern, should_have_transform_with_value_as_identity_matrix)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	const TestPattern p{ white, black };
	EXPECT_EQ(p.transform, matrix_t::identity());
}

/*
Scenario: Assigning a transformation
  Given pattern ← test_pattern()
  When pattern.transform = translation(1, 2, 3)
  Then pattern.transform = translation(1, 2, 3)
*/
TEST(pattern, should_be_able_to_assign_transform)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	TestPattern p{ white, black };
	const matrix_t t{ matrix_t::translation(1, 2, 3) };
	p.transform = t;
	EXPECT_EQ(p.transform, t);
}

/*
Scenario: A pattern with an object transformation
  Given shape ← sphere()
	And s.transform = scaling(2, 2, 2)
	And pattern ← test_pattern()
  When c ← pattern.at_shape(shape, point(2, 3, 4))
  Then c = color(1, 1.5, 2)
*/
TEST(pattern, should_be_able_get_pattern_with_transformed_object)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	TestPattern p{ white, black };
	auto s{ Sphere::create() };
	s->transform = matrix_t::scaling(2, 2, 2);
	const colour_t r{ 1, 1.5, 2 };
	EXPECT_EQ(p.at_object(dynamic_cast<Geometry*>(s.get()), tuple_t::point(2, 3, 4)), r);
}

/*
Scenario: A pattern with a pattern transformation
  Given shape ← sphere()
	And pattern ← test_pattern()
	And pattern.transform = scaling(2, 2, 2)
  When c ← pattern.at_shape(shape, point(2, 3, 4))
  Then c = color(1, 1.5, 2)
*/
TEST(pattern, should_work_with_pattern_transforms)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	TestPattern p{ white, black };
	p.transform = matrix_t::scaling(2, 2, 2);
	auto s{ Sphere::create() };
	const colour_t r{ 1, 1.5, 2 };
	EXPECT_EQ(p.at_object(dynamic_cast<Geometry*>(s.get()), tuple_t::point(2, 3, 4)), r);
}

/*
Scenario: A pattern with both an object and a pattern transformation
  Given shape ← sphere()
	And shape.transform = scaling(2, 2, 2)
	And pattern ← test_pattern()
	And pattern.transform = translation(0.5, 1, 1.5)
  When c ← pattern.at_shape(shape, point(2.5, 3, 3.5))
  Then c = color(0.75, 0.5, 0.25)
*/
TEST(pattern, should_work_with_pattern_and_object_transforms)
{
	const colour_t black{ 0, 0, 0 };
	const colour_t white{ 1, 1, 1 };
	TestPattern p{ white, black };
	p.transform = matrix_t::translation(0.5, 1, 1.5);
	auto s{ Sphere::create() };
	s->transform = matrix_t::scaling(2, 2, 2);
	const colour_t r{ 0.75, 0.5, 0.25 };
	EXPECT_EQ(p.at_object(dynamic_cast<Geometry*>(s.get()), tuple_t::point(2.5, 3, 3.5)), r);
}