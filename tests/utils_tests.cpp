#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../utils.h"

/*
Scenario: Splitting a string by a delimiter
  Given line ← "banana,apple,mango,kiwi"
  When split(line, ",")
  Then the array is
	"""
	banana
	apple
	mango
	kiwi
	"""
*/
TEST(utils, should_splilt_a_string_by_delimiter)
{
	std::string line{ "banana,apple,mango,kiwi" };
	std::vector<std::string> parts{ split(line, ",") };
	EXPECT_EQ(parts[0], std::string{ "banana" });
	EXPECT_EQ(parts[1], std::string{ "apple" });
	EXPECT_EQ(parts[2], std::string{ "mango" });
	EXPECT_EQ(parts[3], std::string{ "kiwi" });
}