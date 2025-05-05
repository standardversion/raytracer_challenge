#include <string>
#include <vector>
#include "gtest/gtest.h"
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

/*
Scenario: Parse input and collect only non-comment tokens
    Given the following multiline input:
      """
      P3
      # this is a comment
      2 1
      255
      255 255 255
      255 0 255 # inline comment
      """
    And the comment symbol is "#"
    When I parse the tokens using the get_clean_tokens function
    Then I should get the following tokens:
      | P3  |
      | 2   |
      | 1   |
      | 255 |
      | 255 |
      | 255 |
      | 255 |
      | 0   |
      | 255 |
*/
TEST(utils, should_parse_tokens_from_a_simulated_PPM_like_input)
{
    std::string input_text =
        "P3\n"
        "# this is a comment\n"
        "2 1\n"
        "255\n"
        "255 255 255\n"
        "255 0 255 # inline comment\n";

    std::istringstream stream(input_text);
    std::vector<std::string> tokens = get_clean_tokens(stream, '#');

    std::vector<std::string> expected = {
        "P3", "2", "1", "255", "255", "255", "255", "255", "0", "255"
    };

    EXPECT_EQ(tokens, expected);
}