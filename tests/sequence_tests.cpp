#include "gtest/gtest.h"
#include "../sequence.h"

/*
Scenario: A number generator returns a cyclic sequence of numbers
  Given gen ← sequence(0.1, 0.5, 1.0)
  Then next(gen) = 0.1
	And next(gen) = 0.5
	And next(gen) = 1.0
	And next(gen) = 0.1
*/
TEST(sequence, should_return_a_cyclic_sequence_of_numbers)
{
	Sequence sequence{ 0.1, 0.5, 1.0 };
	EXPECT_EQ(sequence.next(), 0.1);
	EXPECT_EQ(sequence.next(), 0.5);
	EXPECT_EQ(sequence.next(), 1.0);
	EXPECT_EQ(sequence.next(), 0.1);
}

/*
Scenario: A number generator throws if sequence is empty
  Given gen ← sequence()
  Then next(gen) throws
*/
TEST(sequence, should_throw_if_sequence_is_empty)
{
	Sequence sequence{};
	EXPECT_THROW(sequence.next(), std::invalid_argument);
}