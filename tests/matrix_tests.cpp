#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include "gtest/gtest.h"
#include "../matrix.h"
#include "../tuple.h"

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

/*
Scenario: Constructing a matrix from a tuple
  Given t ← tuple(3, -2, 5, 1)
    And m(t)
  Then M[0,0] = 3
    And M[1,0] = -2
    And M[2,0] = 5
    And M[3,0] = 1
*/
TEST(matrix, should_construct_a_matrix_from_a_tuple)
{
    const tuple_t t{ 3, -2, 5, 1 };
    const matrix_t M{ t };
    EXPECT_EQ(M(0, 0), 3);
    EXPECT_EQ(M(1, 0), -2);
    EXPECT_EQ(M(2, 0), 5);
    EXPECT_EQ(M(3, 0), 1);
}

/*
Scenario: Matrix equality with identical matrices
  Given the following matrix A:
      | 1 | 2 | 3 | 4 |
      | 5 | 6 | 7 | 8 |
      | 9 | 8 | 7 | 6 |
      | 5 | 4 | 3 | 2 |
    And the following matrix B:
      | 1 | 2 | 3 | 4 |
      | 5 | 6 | 7 | 8 |
      | 9 | 8 | 7 | 6 |
      | 5 | 4 | 3 | 2 |
  Then A = B
*/
TEST(matrix, should_be_able_to_compare_matrices)
{
    const std::vector<std::vector<double>> vecs{
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 9, 8, 7, 6 },
        { 5, 4, 3, 2 }
    };
    const matrix_t A{ vecs };
    const matrix_t B{ vecs };
    EXPECT_EQ(A, B);
}

/*
Scenario: Matrix equality with different matrices
  Given the following matrix A:
      | 1 | 2 | 3 | 4 |
      | 5 | 6 | 7 | 8 |
      | 9 | 8 | 7 | 6 |
      | 5 | 4 | 3 | 2 |
    And the following matrix B:
      | 2 | 3 | 4 | 5 |
      | 6 | 7 | 8 | 9 |
      | 8 | 7 | 6 | 5 |
      | 4 | 3 | 2 | 1 |
  Then A != B
*/
TEST(matrix, should_be_able_to_compare_matrices_inequality)
{
    const std::vector<std::vector<double>> vecs{
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 9, 8, 7, 6 },
        { 5, 4, 3, 2 }
    };
    const std::vector<std::vector<double>> vecs2{
        { 2, 3, 4, 5 },
        { 6, 7, 8, 9 },
        { 8, 7, 6, 5 },
        { 4, 3, 2, 1 }
    };
    const matrix_t A{ vecs };
    const matrix_t B{ vecs2 };
    EXPECT_NE(A, B);
}

/*
Scenario: Multiplying two matrices
  Given the following matrix A:
      | 1 | 2 | 3 | 4 |
      | 5 | 6 | 7 | 8 |
      | 9 | 8 | 7 | 6 |
      | 5 | 4 | 3 | 2 |
    And the following matrix B:
      | -2 | 1 | 2 |  3 |
      |  3 | 2 | 1 | -1 |
      |  4 | 3 | 6 |  5 |
      |  1 | 2 | 7 |  8 |
  Then A * B is the following 4x4 matrix:
      | 20|  22 |  50 |  48 |
      | 44|  54 | 114 | 108 |
      | 40|  58 | 110 | 102 |
      | 16|  26 |  46 |  42 |
*/
TEST(matrix, should_be_able_to_multiply_matrices)
{
    const std::vector<std::vector<double>> vecs{
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 9, 8, 7, 6 },
        { 5, 4, 3, 2 }
    };
    const std::vector<std::vector<double>> vecs2{
        { -2, 1, 2, 3 },
        { 3, 2, 1, -1 },
        { 4, 3, 6, 5 },
        { 1, 2, 7, 8 }
    };
    const std::vector<std::vector<double>> vecs3{
        { 20, 22, 50, 48 },
        { 44, 54, 114, 108 },
        { 40, 58, 110, 102 },
        { 16, 26, 46, 42 }
    };
    const matrix_t A{ vecs };
    const matrix_t B{ vecs2 };
    const matrix_t C{ vecs3 };
    EXPECT_EQ(A * B, C);
}

/*
Scenario: A matrix multiplied by a tuple
  Given the following matrix A:
      | 1 | 2 | 3 | 4 |
      | 2 | 4 | 4 | 2 |
      | 8 | 6 | 4 | 1 |
      | 0 | 0 | 0 | 1 |
    And b ← tuple(1, 2, 3, 1)
  Then A * b = tuple(18, 24, 33, 1)
*/
TEST(matrix, should_be_able_to_multiply_matrix_with_tuple)
{
    const std::vector<std::vector<double>> vecs{
        { 1, 2, 3, 4 },
        { 2, 4, 4, 2 },
        { 8, 6, 4, 1 },
        { 0, 0, 0, 1 }
    };
    const matrix_t A{ vecs };
    const tuple_t t{ 1, 2, 3, 1 };
    const tuple_t r{ 18, 24, 33, 1 };
    EXPECT_EQ(A * t, r);
}

/*
Scenario: Multiplying a matrix by the identity matrix
  Given the following matrix A:
    | 0 | 1 |  2 |  4 |
    | 1 | 2 |  4 |  8 |
    | 2 | 4 |  8 | 16 |
    | 4 | 8 | 16 | 32 |
  Then A * identity_matrix = A
*/
TEST(matrix, should_return_same_matrice_on_mlt_by_identity_matrix)
{
    const std::vector<std::vector<double>> vecs{
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 9, 8, 7, 6 },
        { 5, 4, 3, 2 }
    };
    const std::vector<std::vector<double>> vecs2{
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    };

    const matrix_t A{ vecs };
    const matrix_t identity_matrix{ vecs2 };
    EXPECT_EQ(A * identity_matrix, A);
}

/*
Scenario: Multiplying the identity matrix by a tuple
  Given a ← tuple(1, 2, 3, 4)
  Then identity_matrix * a = a
*/
TEST(matrix, should_return_same_tuple_on_mlt_by_identity_matrix)
{
    const tuple_t a{ 1, 2, 3, 4 };
    const std::vector<std::vector<double>> vecs2{
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    };

    const matrix_t identity_matrix{ vecs2 };
    EXPECT_EQ(identity_matrix * a, a);
}

/*
Scenario: Transposing a matrix
  Given the following matrix A:
    | 0 | 9 | 3 | 0 |
    | 9 | 8 | 0 | 8 |
    | 1 | 8 | 5 | 3 |
    | 0 | 0 | 5 | 8 |
  Then transpose(A) is the following matrix:
    | 0 | 9 | 1 | 0 |
    | 9 | 8 | 8 | 0 |
    | 3 | 0 | 5 | 5 |
    | 0 | 8 | 3 | 8 |
*/
TEST(matrix, should_be_able_to_transpose_a_matrix)
{
    const std::vector<std::vector<double>> vecs{
        { 0, 9, 3, 0 },
        { 9, 8, 0, 8 },
        { 1, 8, 5, 3 },
        { 0, 0, 5, 8 }
    };
    const std::vector<std::vector<double>> vecs2{
        { 0, 9, 1, 0 },
        { 9, 8, 8, 0 },
        { 3, 0, 5, 5 },
        { 0, 8, 3, 8 }
    };

    const matrix_t A{ vecs };
    const matrix_t transpose{ vecs2 };
    EXPECT_EQ(A.transpose(), transpose);
}

/*
Scenario: Transposing the identity matrix
  Given A ← transpose(identity_matrix)
  Then A = identity_matrix
*/
TEST(matrix, should_return_identity_matrix_as_transpose_of_identity)
{
    const std::vector<std::vector<double>> vecs2{
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    };

    const matrix_t identity_matrix{ vecs2 };
    EXPECT_EQ(identity_matrix.transpose(), identity_matrix);
}

/*
Scenario: Calculating the determinant of a 2x2 matrix
  Given the following 2x2 matrix A:
    |  1 | 5 |
    | -3 | 2 |
  Then determinant(A) = 17
*/
TEST(matrix, should_return_determinant_of_2x2_matrix)
{
    const std::vector<std::vector<double>> vecs2{
        { 1, 5 },
        { -3, 2 }
    };

    const matrix_t A{ vecs2 };
    EXPECT_EQ(A.determinant(), 17);
}

/*
Scenario: A submatrix of a 3x3 matrix is a 2x2 matrix
  Given the following 3x3 matrix A:
    |  1 | 5 |  0 |
    | -3 | 2 |  7 |
    |  0 | 6 | -3 |
  Then submatrix(A, 0, 2) is the following 2x2 matrix:
    | -3 | 2 |
    |  0 | 6 |
*/
TEST(matrix, should_return_submatrix_of_3x3_matrix)
{
    const std::vector<std::vector<double>> vecs{
        { 1, 5, 0 },
        { -3, 2, 7 },
        { 0, 6, -3 }
    };
    const std::vector<std::vector<double>> vecs2{
        { -3, 2 },
        { 0, 6 }
    };
    const matrix_t M{ vecs };
    const matrix_t sub_matrix{ vecs2 };
    EXPECT_EQ(M.sub_matrix(0, 2), sub_matrix);
}

/*
Scenario: A submatrix of a 4x4 matrix is a 3x3 matrix
  Given the following 4x4 matrix A:
    | -6 |  1 |  1 |  6 |
    | -8 |  5 |  8 |  6 |
    | -1 |  0 |  8 |  2 |
    | -7 |  1 | -1 |  1 |
  Then submatrix(A, 2, 1) is the following 3x3 matrix:
    | -6 |  1 | 6 |
    | -8 |  8 | 6 |
    | -7 | -1 | 1 |
*/
TEST(matrix, should_return_submatrix_of_4x4_matrix)
{
    const std::vector<std::vector<double>> vecs{
        { -6, 1, 1, 6 },
        { -8, 5, 8, 6 },
        { -1, 0, 8, 2 },
        { -7, 1, -1, 1 }
    };
    const std::vector<std::vector<double>> vecs2{
        { -6, 1, 6 },
        { -8, 8, 6 },
        { -7, -1, 1 }
    };
    const matrix_t M{ vecs };
    const matrix_t sub_matrix{ vecs2 };
    EXPECT_EQ(M.sub_matrix(2, 1), sub_matrix);
}

/*
Scenario: Calculating the determinant of a 3x3 matrix
  Given the following 3x3 matrix A:
    |  1 |  2 |  6 |
    | -5 |  8 | -4 |
    |  2 |  6 |  4 |
  Then A.determinant = -196
*/
TEST(matrix, should_return_determinant_of_3x3_matrix)
{
    const std::vector<std::vector<double>> vecs{
        { 1, 2, 6 },
        { -5, 8, -4 },
        { 2, 6, 4 }
    };
    const matrix_t A{ vecs };
    EXPECT_EQ(A.determinant(), -196);
}

/*
Scenario: Calculating the determinant of a 4x4 matrix
  Given the following 4x4 matrix A:
    | -2 | -8 |  3 |  5 |
    | -3 |  1 |  7 |  3 |
    |  1 |  2 | -9 |  6 |
    | -6 |  7 |  7 | -9 |
  Then A.determinant() = -4071
*/
TEST(matrix, should_return_determinant_of_4x4_matrix)
{
    const std::vector<std::vector<double>> vecs{
        { -2, -8, 3, 5 },
        { -3, 1, 7, 3 },
        { 1, 2, -9, 6 },
        { -6, 7, 7, -9 }
    };
    const matrix_t A{ vecs };
    EXPECT_EQ(A.determinant(), -4071);
}

/*
Scenario: Testing an invertible matrix for invertibility
  Given the following 4x4 matrix A:
    |  6 |  4 |  4 |  4 |
    |  5 |  5 |  7 |  6 |
    |  4 | -9 |  3 | -7 |
    |  9 |  1 |  7 | -6 |
  Then A.determinant() = -2120
    And A.inverse() does not throw
*/
TEST(matrix, should_not_throw_if_determinant_is_ne_0_for_inverse)
{
    const std::vector<std::vector<double>> vecs{
        { 6, 4, 4, 4 },
        { 5, 5, 7, 6 },
        { 4, -9, 3, -7 },
        { 9, 1, 7, -6 }
    };
    const matrix_t A{ vecs };
    EXPECT_NO_THROW(A.inverse());
}

/*
Scenario: Testing a noninvertible matrix for invertibility
  Given the following 4x4 matrix A:
    | -4 |  2 | -2 | -3 |
    |  9 |  6 |  2 |  6 |
    |  0 | -5 |  1 | -5 |
    |  0 |  0 |  0 |  0 |
  Then A.determinant() = 0
    And A.inverse() throws invalid argument
*/
TEST(matrix, should_throw_if_determinant_is_ne_0_for_inverse)
{
    const std::vector<std::vector<double>> vecs{
        { -4, 2, -2, -3 },
        { 9, 6, 2, 6 },
        { 0, -5, 1, -5 },
        { 0, 0, 0, 0 }
    };
    const matrix_t A{ vecs };
    EXPECT_NO_THROW(A.inverse());
}

/*
Scenario: Calculating the inverse of a matrix
  Given the following 4x4 matrix A:
      | -5 |  2 |  6 | -8 |
      |  1 | -5 |  1 |  8 |
      |  7 |  7 | -6 | -7 |
      |  1 | -3 |  7 |  4 |
    And B ← A.inverse()
  Then  B is the following 4x4 matrix:
      |  0.21805 |  0.45113 |  0.24060 | -0.04511 |
      | -0.80827 | -1.45677 | -0.44361 |  0.52068 |
      | -0.07895 | -0.22368 | -0.05263 |  0.19737 |
      | -0.52256 | -0.81391 | -0.30075 |  0.30639 |
*/
TEST(matrix, should_calculate_matrix_inverse)
{
    const std::vector<std::vector<double>> vecs{
        { -5, 2, 6, -8 },
        { 1, -5, 1, 8 },
        { 7, 7, -6, -7 },
        { 1, -3, 7, 4 }
    };
    const std::vector<std::vector<double>> vecs2{
        { 0.21805, 0.45113, 0.24060, -0.04511 },
        { -0.80827, -1.45677, -0.44361, 0.52068 },
        { -0.07895, -0.22368, -0.05263, 0.19737 },
        { -0.52256, -0.81391, -0.30075, 0.30639 }
    };
    const matrix_t A{ vecs };
    const matrix_t B{ vecs2 };
    EXPECT_EQ(A.inverse(), B);
}

/*
Scenario: Calculating the inverse of another matrix
  Given the following 4x4 matrix A:
    |  8 | -5 |  9 |  2 |
    |  7 |  5 |  6 |  1 |
    | -6 |  0 |  9 |  6 |
    | -3 |  0 | -9 | -4 |
  Then A.inverse() is the following 4x4 matrix:
    | -0.15385 | -0.15385 | -0.28205 | -0.53846 |
    | -0.07692 |  0.12308 |  0.02564 |  0.03077 |
    |  0.35897 |  0.35897 |  0.43590 |  0.92308 |
    | -0.69231 | -0.69231 | -0.76923 | -1.92308 |
*/
TEST(matrix, should_calculate_matrix_inverse2)
{
    const std::vector<std::vector<double>> vecs{
        { 8, -5, 9, 2 },
        { 7, 5, 6, 1 },
        { -6, 0, 9, 6 },
        { -3, 0, -9, -4 }
    };
    const std::vector<std::vector<double>> vecs2{
        { -0.15385, -0.15385, -0.28205, -0.53846 },
        { -0.07692, 0.12308, 0.02564, 0.03077 },
        { 0.35897, 0.35897, 0.43590, 0.92308 },
        { -0.69231, -0.69231, -0.76923, -1.92308 }
    };
    const matrix_t A{ vecs };
    const matrix_t B{ vecs2 };
    EXPECT_EQ(A.inverse(), B);
}

/*
Scenario: Calculating the inverse of a third matrix
  Given the following 4x4 matrix A:
    |  9 |  3 |  0 |  9 |
    | -5 | -2 | -6 | -3 |
    | -4 |  9 |  6 |  4 |
    | -7 |  6 |  6 |  2 |
  Then A.inverse() is the following 4x4 matrix:
    | -0.04074 | -0.07778 |  0.14444 | -0.22222 |
    | -0.07778 |  0.03333 |  0.36667 | -0.33333 |
    | -0.02901 | -0.14630 | -0.10926 |  0.12963 |
    |  0.17778 |  0.06667 | -0.26667 |  0.33333 |
*/
TEST(matrix, should_calculate_matrix_inverse3)
{
    const std::vector<std::vector<double>> vecs{
        { 9, 3, 0, 9 },
        { -5, -2, -6, -3 },
        { -4, 9, 6, 4 },
        { -7, 6, 6, 2 }
    };
    const std::vector<std::vector<double>> vecs2{
        { -0.04074, -0.07778, 0.14444, -0.22222 },
        { -0.07778, 0.03333, 0.36667, -0.33333 },
        { -0.02901, -0.14630, -0.10926, 0.12963 },
        { 0.17778, 0.06667, -0.26667, 0.33333 }
    };
    const matrix_t A{ vecs };
    const matrix_t B{ vecs2 };
    EXPECT_EQ(A.inverse(), B);
}

/*
Scenario: Multiplying a product by its inverse
  Given the following 4x4 matrix A:
      |  3 | -9 |  7 |  3 |
      |  3 | -8 |  2 | -9 |
      | -4 |  4 |  4 |  1 |
      | -6 |  5 | -1 |  1 |
    And the following 4x4 matrix B:
      |  8 |  2 |  2 |  2 |
      |  3 | -1 |  7 |  0 |
      |  7 |  0 |  5 |  4 |
      |  6 | -2 |  0 |  5 |
    And C ← A * B
  Then C * B.inverse() = A
*/
TEST(matrix, should_get_og_matrix_when_mult_prod_by_inverse)
{
    const std::vector<std::vector<double>> vecs{
        { 3, -9, 7, 3 },
        { 3, -8, 2, -9 },
        { -4, 4, 4, 1 },
        { -6, 5, -1, 1 }
    };
    const std::vector<std::vector<double>> vecs2{
        { 8, 2, 2, 2 },
        { 3, -1, 7, 0 },
        { 7, 0, 5, 4 },
        { 6, -2, 0, 5 }
    };
    const matrix_t A{ vecs };
    const matrix_t B{ vecs2 };
    const matrix_t C{ A * B };
    EXPECT_EQ(C * B.inverse(), A);
}

/*
Scenario: Multiplying by a translation matrix
  Given transform ← translation(5, -3, 2)
    And p ← point(-3, 4, 5)
   Then transform * p = point(2, 1, 7)
*/
TEST(matrix, should_create_translation_4x4_matrix_and_multiply_the_point)
{
    const matrix_t transform{ matrix_t::translation(5, -3, 2) };
    const tuple_t p{ tuple_t::point(-3, 4, 5) };
    const tuple_t r{ tuple_t::point(2, 1, 7) };
    EXPECT_EQ(transform * p, r);
}

/*
Scenario: Multiplying by the inverse of a translation matrix
  Given transform ← translation(5, -3, 2)
    And inv ← transform.inverse()
    And p ← point(-3, 4, 5)
   Then inv * p = point(-8, 7, 3)
*/
TEST(matrix, should_reverse_the_translation_when_multilpied_by_inverse_translation)
{
    const matrix_t transform{ matrix_t::translation(5, -3, 2) };
    const matrix_t inverse{ transform.inverse() };
    const tuple_t p{ tuple_t::point(-3, 4, 5) };
    const tuple_t r{ tuple_t::point(-8, 7, 3) };
    EXPECT_EQ(inverse * p, r);
}

/*
Scenario: Translation does not affect vectors
  Given transform ← translation(5, -3, 2)
    And v ← vector(-3, 4, 5)
   Then transform * v = v
*/
TEST(matrix, should_return_same_vector_when_multilpied_by_translation)
{
    const matrix_t transform{ matrix_t::translation(5, -3, 2) };
    const tuple_t v{ tuple_t::vector(-3, 4, 5) };
    EXPECT_EQ(transform * v, v);
}

/*
Scenario: A scaling matrix applied to a point
  Given transform ← scaling(2, 3, 4)
    And p ← point(-4, 6, 8)
   Then transform * p = point(-8, 18, 32)
*/
TEST(matrix, should_scale_a_point)
{
    const matrix_t transform{ matrix_t::scaling(2, 3, 4) };
    const tuple_t p{ tuple_t::point(-4, 6, 8) };
    const tuple_t r{ tuple_t::point(-8, 18, 32) };
    EXPECT_EQ(transform * p, r);
}

/*
Scenario: A scaling matrix applied to a vector
  Given transform ← scaling(2, 3, 4)
    And v ← vector(-4, 6, 8)
   Then transform * v = vector(-8, 18, 32)
*/
TEST(matrix, should_scale_a_vector)
{
    const matrix_t transform{ matrix_t::scaling(2, 3, 4) };
    const tuple_t v{ tuple_t::vector(-4, 6, 8) };
    const tuple_t r{ tuple_t::vector(-8, 18, 32) };
    EXPECT_EQ(transform * v, r);
}

/*
Scenario: Multiplying by the inverse of a scaling matrix
  Given transform ← scaling(2, 3, 4)
    And inv ← inverse(transform)
    And v ← vector(-4, 6, 8)
   Then inv * v = vector(-2, 2, 2)
*/
TEST(matrix, should_shrink_when_scaled_by_inverse)
{
    const matrix_t transform{ matrix_t::scaling(2, 3, 4) };
    const matrix_t inverse{ transform.inverse() };
    const tuple_t v{ tuple_t::vector(-4, 6, 8) };
    const tuple_t r{ tuple_t::vector(-2, 2, 2) };
    EXPECT_EQ(inverse * v, r);
}

/*
Scenario: Reflection is scaling by a negative value
  Given transform ← scaling(-1, 1, 1)
    And p ← point(2, 3, 4)
   Then transform * p = point(-2, 3, 4)
*/
TEST(matrix, should_reflect_a_point_when_scaled_by_negative_value)
{
    const matrix_t transform{ matrix_t::scaling(-1, 1, 1) };
    const tuple_t p{ tuple_t::point(2, 3, 4) };
    const tuple_t r{ tuple_t::point(-2, 3, 4) };
    EXPECT_EQ(transform * p, r);
}

/*
Scenario: Rotating a point around the x axis
  Given p ← point(0, 1, 0)
    And half_quarter ← rotation_x(π / 4)
    And full_quarter ← rotation_x(π / 2)
  Then half_quarter * p = point(0, √2/2, √2/2)
    And full_quarter * p = point(0, 0, 1)
*/
TEST(matrix, should_rotate_a_point_around_x_axis)
{
    const tuple_t p{ tuple_t::point(0, 1, 0) };
    const matrix_t half_quarter{ matrix_t::rotation_x(M_PI / 4) };
    const matrix_t full_quarter{ matrix_t::rotation_x(M_PI / 2) };
    const tuple_t r1{ tuple_t::point(0, std::sqrt(2) / 2, std::sqrt(2) / 2)};
    const tuple_t r2{ tuple_t::point(0, 0, 1) };
    EXPECT_EQ(half_quarter * p, r1);
    EXPECT_EQ(full_quarter * p, r2);
}

/*
Scenario: The inverse of an x-rotation rotates in the opposite direction
  Given p ← point(0, 1, 0)
    And half_quarter ← rotation_x(π / 4)
    And inv ← inverse(half_quarter)
  Then inv * p = point(0, √2/2, -√2/2)
*/
TEST(matrix, should_rotate_in_opposite_direction_when_rotated_by_inverse)
{
    const tuple_t p{ tuple_t::point(0, 1, 0) };
    const matrix_t half_quarter{ matrix_t::rotation_x(M_PI / 4) };
    const matrix_t inverse{ half_quarter.inverse() };
    const tuple_t r{ tuple_t::point(0, std::sqrt(2) / 2, -std::sqrt(2) / 2) };
    EXPECT_EQ(inverse * p, r);
}

/*
Scenario: Rotating a point around the y axis
  Given p ← point(0, 0, 1)
    And half_quarter ← rotation_y(π / 4)
    And full_quarter ← rotation_y(π / 2)
  Then half_quarter * p = point(√2/2, 0, √2/2)
    And full_quarter * p = point(1, 0, 0)
*/
TEST(matrix, should_rotate_a_point_around_y_axis)
{
    const tuple_t p{ tuple_t::point(0, 0, 1) };
    const matrix_t half_quarter{ matrix_t::rotation_y(M_PI / 4) };
    const matrix_t full_quarter{ matrix_t::rotation_y(M_PI / 2) };
    const tuple_t r1{ tuple_t::point(std::sqrt(2) / 2, 0, std::sqrt(2) / 2) };
    const tuple_t r2{ tuple_t::point(1, 0, 0) };
    EXPECT_EQ(half_quarter * p, r1);
    EXPECT_EQ(full_quarter * p, r2);
}

/*
Scenario: Rotating a point around the z axis
  Given p ← point(0, 1, 0)
    And half_quarter ← rotation_z(π / 4)
    And full_quarter ← rotation_z(π / 2)
  Then half_quarter * p = point(-√2/2, √2/2, 0)
    And full_quarter * p = point(-1, 0, 0)
*/
TEST(matrix, should_rotate_a_point_around_z_axis)
{
    const tuple_t p{ tuple_t::point(0, 1, 0) };
    const matrix_t half_quarter{ matrix_t::rotation_z(M_PI / 4) };
    const matrix_t full_quarter{ matrix_t::rotation_z(M_PI / 2) };
    const tuple_t r1{ tuple_t::point(-std::sqrt(2) / 2, std::sqrt(2) / 2, 0) };
    const tuple_t r2{ tuple_t::point(-1, 0, 0) };
    EXPECT_EQ(half_quarter * p, r1);
    EXPECT_EQ(full_quarter * p, r2);
}

/*
Scenario: A shearing transformation moves x in proportion to y
  Given transform ← shearing(1, 0, 0, 0, 0, 0)
    And p ← point(2, 3, 4)
  Then transform * p = point(5, 3, 4)
*/
TEST(matrix, should_move_x_in_proportion_to_y_when_sheared)
{
    const matrix_t transform{ matrix_t::shearing(1, 0, 0, 0, 0, 0) };
    const tuple_t p{ tuple_t::point(2, 3, 4) };
    const tuple_t r{ tuple_t::point(5, 3, 4) };
    EXPECT_EQ(transform * p, r);
}

/*
Scenario: A shearing transformation moves x in proportion to z
  Given transform ← shearing(0, 1, 0, 0, 0, 0)
    And p ← point(2, 3, 4)
  Then transform * p = point(6, 3, 4)
*/
TEST(matrix, should_move_x_in_proportion_to_z_when_sheared)
{
    const matrix_t transform{ matrix_t::shearing(0, 1, 0, 0, 0, 0) };
    const tuple_t p{ tuple_t::point(2, 3, 4) };
    const tuple_t r{ tuple_t::point(6, 3, 4) };
    EXPECT_EQ(transform * p, r);
}

/*
Scenario: A shearing transformation moves y in proportion to x
  Given transform ← shearing(0, 0, 1, 0, 0, 0)
    And p ← point(2, 3, 4)
  Then transform * p = point(2, 5, 4)
*/
TEST(matrix, should_move_y_in_proportion_to_x_when_sheared)
{
    const matrix_t transform{ matrix_t::shearing(0, 0, 1, 0, 0, 0) };
    const tuple_t p{ tuple_t::point(2, 3, 4) };
    const tuple_t r{ tuple_t::point(2, 5, 4) };
    EXPECT_EQ(transform * p, r);
}

/*
Scenario: A shearing transformation moves y in proportion to z
  Given transform ← shearing(0, 0, 0, 1, 0, 0)
    And p ← point(2, 3, 4)
  Then transform * p = point(2, 7, 4)
*/
TEST(matrix, should_move_y_in_proportion_to_z_when_sheared)
{
    const matrix_t transform{ matrix_t::shearing(0, 0, 0, 1, 0, 0) };
    const tuple_t p{ tuple_t::point(2, 3, 4) };
    const tuple_t r{ tuple_t::point(2, 7, 4) };
    EXPECT_EQ(transform * p, r);
}

/*
Scenario: A shearing transformation moves z in proportion to x
  Given transform ← shearing(0, 0, 0, 0, 1, 0)
    And p ← point(2, 3, 4)
  Then transform * p = point(2, 3, 6)
*/
TEST(matrix, should_move_z_in_proportion_to_x_when_sheared)
{
    const matrix_t transform{ matrix_t::shearing(0, 0, 0, 0, 1, 0) };
    const tuple_t p{ tuple_t::point(2, 3, 4) };
    const tuple_t r{ tuple_t::point(2, 3, 6) };
    EXPECT_EQ(transform * p, r);
}

/*
Scenario: A shearing transformation moves z in proportion to y
  Given transform ← shearing(0, 0, 0, 0, 0, 1)
    And p ← point(2, 3, 4)
  Then transform * p = point(2, 3, 7)
*/
TEST(matrix, should_move_z_in_proportion_to_y_when_sheared)
{
    const matrix_t transform{ matrix_t::shearing(0, 0, 0, 0, 0, 1) };
    const tuple_t p{ tuple_t::point(2, 3, 4) };
    const tuple_t r{ tuple_t::point(2, 3, 7) };
    EXPECT_EQ(transform * p, r);
}

/*
Scenario: Individual transformations are applied in sequence
  Given p ← point(1, 0, 1)
    And A ← rotation_x(π / 2)
    And B ← scaling(5, 5, 5)
    And C ← translation(10, 5, 7)
  # apply rotation first
  When p2 ← A * p
  Then p2 = point(1, -1, 0)
  # then apply scaling
  When p3 ← B * p2
  Then p3 = point(5, -5, 0)
  # then apply translation
  When p4 ← C * p3
  Then p4 = point(15, 0, 7)
*/
TEST(matrix, should_apply_individual_transforms_sequntially)
{
    const tuple_t p{ tuple_t::point(1, 0, 1) };
    const matrix_t A{ matrix_t::rotation_x(M_PI / 2)};
    const matrix_t B{ matrix_t::scaling(5, 5, 5) };
    const matrix_t C{ matrix_t::translation(10, 5, 7) };
    const tuple_t p2{ tuple_t::point(1, -1, 0) };
    const tuple_t p3{ tuple_t::point(5, -5, 0) };
    const tuple_t p4{ tuple_t::point(15, 0, 7) };
    EXPECT_EQ(A * p, p2);
    EXPECT_EQ(B * p2, p3);
    EXPECT_EQ(C * p3, p4);
}

/*
Scenario: Chained transformations must be applied in reverse order
  Given p ← point(1, 0, 1)
    And A ← rotation_x(π / 2)
    And B ← scaling(5, 5, 5)
    And C ← translation(10, 5, 7)
  When T ← C * B * A
  Then T * p = point(15, 0, 7)
*/
TEST(matrix, should_apply_chained_transforms_in_reverse_order)
{
    const tuple_t p{ tuple_t::point(1, 0, 1) };
    const matrix_t A{ matrix_t::rotation_x(M_PI / 2) };
    const matrix_t B{ matrix_t::scaling(5, 5, 5) };
    const matrix_t C{ matrix_t::translation(10, 5, 7) };
    const matrix_t transform{ C * B * A };
    const tuple_t r{ tuple_t::point(15, 0, 7) };
    EXPECT_EQ(transform * p, r);
}

/*
Scenario: The transformation matrix for the default orientation
  Given from ← point(0, 0, 0)
    And to ← point(0, 0, -1)
    And up ← vector(0, 1, 0)
  When t ← view_transform(from, to, up)
  Then t = identity_matrix
*/
TEST(matrix, should_return_matrix_for_default_orientation)
{
    const tuple_t from{ tuple_t::point(0, 0, 0) };
    const tuple_t to{ tuple_t::point(0, 0, -1) };
    const tuple_t up{ tuple_t::vector(0, 1, 0) };
    const matrix_t t{ matrix_t::view_transform(from, to, up) };
    const matrix_t r{ matrix_t::identity() };
    EXPECT_EQ(t, r);
}

/*
Scenario: A view transformation matrix looking in positive z direction
  Given from ← point(0, 0, 0)
    And to ← point(0, 0, 1)
    And up ← vector(0, 1, 0)
  When t ← view_transform(from, to, up)
  Then t = scaling(-1, 1, -1)
*/
TEST(matrix, should_return_matrix_for_looking_in_positive_z_direction)
{
    const tuple_t from{ tuple_t::point(0, 0, 0) };
    const tuple_t to{ tuple_t::point(0, 0, 1) };
    const tuple_t up{ tuple_t::vector(0, 1, 0) };
    const matrix_t t{ matrix_t::view_transform(from, to, up) };
    const matrix_t r{ matrix_t::scaling(-1, 1, -1) };
    EXPECT_EQ(t, r);
}

/*
Scenario: The view transformation moves the world
  Given from ← point(0, 0, 8)
    And to ← point(0, 0, 0)
    And up ← vector(0, 1, 0)
  When t ← view_transform(from, to, up)
  Then t = translation(0, 0, -8)
*/
TEST(matrix, should_return_matrix_which_moves_the_world)
{
    const tuple_t from{ tuple_t::point(0, 0, 8) };
    const tuple_t to{ tuple_t::point(0, 0, 0) };
    const tuple_t up{ tuple_t::vector(0, 1, 0) };
    const matrix_t t{ matrix_t::view_transform(from, to, up) };
    const matrix_t r{ matrix_t::translation(0, 0, -8) };
    EXPECT_EQ(t, r);
}

/*
Scenario: An arbitrary view transformation
  Given from ← point(1, 3, 2)
    And to ← point(4, -2, 8)
    And up ← vector(1, 1, 0)
  When t ← view_transform(from, to, up)
  Then t is the following 4x4 matrix:
      | -0.50709 | 0.50709 |  0.67612 | -2.36643 |
      |  0.76772 | 0.60609 |  0.12122 | -2.82843 |
      | -0.35857 | 0.59761 | -0.71714 |  0.00000 |
      |  0.00000 | 0.00000 |  0.00000 |  1.00000 |
*/
TEST(matrix, should_return_matrix_which_represents_arbitrary_view_transformation)
{
    const tuple_t from{ tuple_t::point(1, 3, 2) };
    const tuple_t to{ tuple_t::point(4, -2, 8) };
    const tuple_t up{ tuple_t::vector(1, 1, 0) };
    const matrix_t t{ matrix_t::view_transform(from, to, up) };
    const std::vector<std::vector<double>> vecs{
        { -0.50709, 0.50709, 0.67612, -2.36643 },
        { 0.76772, 0.60609, 0.12122, -2.82843 },
        { -0.35857, 0.59761, -0.71714, 0.00000 },
        { 0.00000, 0.00000, 0.00000, 1.00000 }
    };
    const matrix_t r{ vecs };
    EXPECT_EQ(t, r);
}