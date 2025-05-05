#pragma once
#include <vector>
#include <string>

/**
 * @brief Splits a string into a vector of substrings based on a delimiter.
 *
 * This function takes a string and splits it into multiple substrings, using the
 * specified delimiter to determine where each split occurs. The delimiter itself
 * is not included in the resulting substrings.
 *
 * @param str The input string to be split.
 * @param delimiter The string used as the delimiter to split the input string.
 *                  This can be a single character or a multi-character string.
 *
 * @return A `std::vector<std::string>` containing the substrings resulting from
 *         the split operation. If the delimiter is not found in the string, the
 *         entire string is returned as a single element in the vector.
 *
 * @note If the delimiter is empty, the function's behavior is undefined or may
 *       result in unexpected behavior.
 */
std::vector<std::string> split(const std::string& str, const std::string& delimeter);

/**
 * @brief Reads and tokenizes an input stream, removing comments and whitespace.
 *
 * This function processes the input stream, breaking it into a list of tokens
 * (strings) while ignoring any comments and extra whitespace. By default, it
 * treats lines starting with the specified `comment_symbol` as comments and skips them.
 *
 * @param input The input stream to read from. Typically a file or string stream.
 * @param comment_symbol The character that starts a comment (default is `#`).
 *
 * @return A vector of strings, each representing a cleaned token from the input.
 *         Comments and empty lines are excluded from the returned list of tokens.
 *
 * @note If the input stream contains a comment, the function ignores the entire
 *       line starting from the comment symbol.
 *
 * @example
 * std::ifstream file("example.txt");
 * std::vector<std::string> tokens = get_clean_tokens(file, '#');
 */
std::vector<std::string> get_clean_tokens(std::istream& input, const char comment_symbol = '#');
