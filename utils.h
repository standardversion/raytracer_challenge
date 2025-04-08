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
