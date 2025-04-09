#include <vector>
#include "gtest/gtest.h"
#include "../matrix.h"

/*
Scenario: An empty 3x3 matrix
  Given M(3, 3)
  Then M.data.size() = 3
    And M.data[0].size() = 3
*/
TEST(matrix, should_construct_a_3x3_matrix_of_zeros)
{

    const matrix_t M{ 3, 3 };
    EXPECT_EQ(M.data.size(), 3);
    EXPECT_EQ(M.data[0].size(), 3);
    EXPECT_EQ(M(0, 0), 0);
}

/*
Scenario: Invalid construction args
  Given M(-3, 0)
  Then constructor throws
*/
TEST(matrix, should_throw_if_constructing_with_invalid_args)
{
    EXPECT_THROW(matrix_t M(- 3, 0), std::invalid_argument);
}

/*
Scenario: Invalid construction args
  Given v ← std::vector<std::vector<double>>{{}}
  Then constructor throws
*/
TEST(matrix, should_throw_if_constructing_with_invalid_vec_args)
{
    const std::vector<std::vector<double>> v{ {} };
    EXPECT_THROW(matrix_t M(v), std::invalid_argument);
}

/*
Scenario: Inconsistent construction args
  Given v ← std::vector<std::vector<double>>{{}}
  Then constructor throws
*/
TEST(matrix, should_throw_if_constructing_with_inconsistent_vec_args)
{
    const std::vector<std::vector<double>> v{ {1, 0}, {1} };
    EXPECT_THROW(matrix_t M(v), std::invalid_argument);
}

/*
Scenario: Constructing and inspecting a 4x4 matrix
  Given the following 4x4 matrix M:
    |  1   |  2   |  3   |  4   |
    |  5.5 |  6.5 |  7.5 |  8.5 |
    |  9   | 10   | 11   | 12   |
    | 13.5 | 14.5 | 15.5 | 16.5 |
  Then M[0,0] = 1
    And M[0,3] = 4
    And M[1,0] = 5.5
    And M[1,2] = 7.5
    And M[2,2] = 11
    And M[3,0] = 13.5
    And M[3,2] = 15.5
*/
TEST(matrix, should_construct_a_4x4_matrix)
{
    const std::vector<std::vector<double>> vecs{
        { 1, 2, 3, 4 },
        { 5.5, 6.5, 7.5, 8.5 },
        { 9, 10, 11, 12 },
        { 13.5, 14.5, 15.5, 16.5 }
    };
    const matrix_t M{ vecs };
    EXPECT_EQ(M(0, 0), 1);
    EXPECT_EQ(M(0, 3), 4);
    EXPECT_EQ(M(1, 0), 5.5);
    EXPECT_EQ(M(1, 2), 7.5);
    EXPECT_EQ(M(2, 2), 11);
    EXPECT_EQ(M(3, 0), 13.5);
    EXPECT_EQ(M(3, 2), 15.5);
}

/*
Scenario: A 2x2 matrix ought to be representable
  Given the following 2x2 matrix M:
    | -3 |  5 |
    |  1 | -2 |
  Then M[0,0] = -3
    And M[0,1] = 5
    And M[1,0] = 1
    And M[1,1] = -2
*/
TEST(matrix, should_construct_a_2x2_matrix)
{
    const std::vector<std::vector<double>> vecs{
        { -3, 5 },
        { 1, -2 }
    };
    const matrix_t M{ vecs };
    EXPECT_EQ(M(0, 0), -3);
    EXPECT_EQ(M(0, 1), 5);
    EXPECT_EQ(M(1, 0), 1);
    EXPECT_EQ(M(1, 1), -2);
}

/*
Scenario: A 3x3 matrix ought to be representable
  Given the following 3x3 matrix M:
    | -3 |  5 |  0 |
    |  1 | -2 | -7 |
    |  0 |  1 |  1 |
  Then M[0,0] = -3
    And M[1,1] = -2
    And M[2,2] = 1
*/
TEST(matrix, should_construct_a_3x3_matrix)
{
    const std::vector<std::vector<double>> vecs{
        { -3, 5, 0 },
        { 1, -2, -7 },
        { 0, 1, 1 }
    };
    const matrix_t M{ vecs };
    EXPECT_EQ(M(0, 0), -3);
    EXPECT_EQ(M(1, 1), -2);
    EXPECT_EQ(M(2, 2), 1);
}