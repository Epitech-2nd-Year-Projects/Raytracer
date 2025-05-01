/**
 * @file Matrix.hpp
 * @brief Defines the Matrix class template for representing mathematical
 * matrices.
 */
#pragma once

#include "Exceptions/RaytracerException.hpp"
#include "Math/Point.hpp"
#include "Math/Vector.hpp"
#include <array>
#include <cmath>
#include <cstddef>
#include <initializer_list>

namespace Raytracer::Math {

/**
 * @class Matrix
 * @brief A class template representing a mathematical matrix.
 * @tparam Rows The number of rows in the matrix.
 * @tparam Cols The number of columns in the matrix.
 * @tparam T The type of the matrix elements (default: double).
 */
template <std::size_t Rows, std::size_t Cols, typename T = double>
class Matrix {
public:
  /**
   * @brief Default constructor initializes matrix with zeros.
   */
  Matrix() noexcept {
    for (auto &row : m_matrix) {
      row.fill(T{});
    }
  }

  /**
   * @brief Construct matrix from nested initializer lists.
   * @param list Nested initializer lists representing matrix elements.
   */
  Matrix(const std::initializer_list<std::initializer_list<T>> &list) {
    if (list.size() > Rows) {
      throw Exceptions::RaytracerException("Too many rows in initializer list");
    }

    std::size_t iRow = 0;
    for (const auto &row : list) {
      if (row.size() > Cols) {
        throw Exceptions::RaytracerException(
            "Too many columns in initializer list");
      }

      std::size_t iCol = 0;
      for (const auto &col : row) {
        m_matrix[iRow][iCol] = col;
        iCol++;
      }
      for (; iCol < Cols; iCol++) {
        m_matrix[iRow][iCol] = T{};
      }
      iRow++;
    }
    for (; iRow < Rows; iRow++) {
      m_matrix[iRow].fill(T{});
    }
  }

  /**
   * @brief Copy constructor.
   */
  Matrix(const Matrix &other) noexcept = default;

  /**
   * @brief Assignment operator.
   */
  Matrix &operator=(const Matrix &other) noexcept = default;

  /**
   * @brief Access matrix element (mutable).
   * @param row Row index.
   * @param col Column index.
   * @return Reference to the element.
   */
  T &operator()(const std::size_t row, const std::size_t col) noexcept {
    return m_matrix[row][col];
  }

  /**
   * @brief Access matrix element (const).
   * @param row Row index.
   * @param col Column index.
   * @return Const reference to the element.
   */
  const T &operator()(const std::size_t row,
                      const std::size_t col) const noexcept {
    return m_matrix[row][col];
  }

  /**
   * @brief Add another matrix to this one.
   * @param other The matrix to add.
   * @return Reference to this matrix after addition.
   */
  Matrix &operator+=(const Matrix &other) noexcept {
    for (std::size_t iRow = 0; iRow < Rows; iRow++) {
      for (std::size_t iCol = 0; iCol < Cols; iCol++) {
        m_matrix[iRow][iCol] += other.m_matrix[iRow][iCol];
      }
    }
    return *this;
  }

  /**
   * @brief Subtract another matrix from this one.
   * @param other The matrix to subtract.
   * @return Reference to this matrix after subtraction.
   */
  Matrix &operator-=(const Matrix &other) noexcept {
    for (std::size_t iRow = 0; iRow < Rows; iRow++) {
      for (std::size_t iCol = 0; iCol < Cols; iCol++) {
        m_matrix[iRow][iCol] -= other.m_matrix[iRow][iCol];
      }
    }
    return *this;
  }

  /**
   * @brief Multiply this matrix by a scalar.
   * @param scalar The scalar value.
   * @return Reference to this matrix after multiplication.
   */
  Matrix &operator*=(const T scalar) noexcept {
    for (std::size_t iRow = 0; iRow < Rows; iRow++) {
      for (std::size_t iCol = 0; iCol < Cols; iCol++) {
        m_matrix[iRow][iCol] *= scalar;
      }
    }
    return *this;
  }

  /**
   * @brief Add two matrices.
   * @param other The matrix to add.
   * @return New matrix representing the sum.
   */
  Matrix operator+(const Matrix &other) const noexcept {
    Matrix result(*this);
    result += other;
    return result;
  }

  /**
   * @brief Subtract two matrices.
   * @param other The matrix to subtract.
   * @return New matrix representing the difference.
   */
  Matrix operator-(const Matrix &other) const noexcept {
    Matrix result(*this);
    result -= other;
    return result;
  }

  /**
   * @brief Multiply matrix by a scalar.
   * @param scalar The scalar value.
   * @return New matrix representing the product.
   */
  Matrix operator*(const T scalar) const noexcept {
    Matrix result(*this);
    result *= scalar;
    return result;
  }

  /**
   * @brief Multiply two matrices.
   * @tparam OtherCols The number of columns in the other matrix.
   * @param other The matrix to multiply with.
   * @return New matrix representing the product.
   */
  template <std::size_t OtherCols>
  Matrix<Rows, OtherCols, T>
  multiply(const Matrix<Cols, OtherCols, T> &other) const noexcept {
    Matrix<Rows, OtherCols, T> result;
    for (std::size_t i = 0; i < Rows; i++) {
      for (std::size_t j = 0; j < OtherCols; j++) {
        T sum = T{};
        for (std::size_t k = 0; k < Cols; k++) {
          sum += (*this)(i, k) * other(k, j);
        }
        result(i, j) = sum;
      }
    }
    return result;
  }

  /**
   * @brief Create an identity matrix.
   * @return Identity matrix.
   */
  static Matrix identity() noexcept {
    static_assert(Rows == Cols, "Identity matrix must be square");
    Matrix result;
    for (std::size_t i = 0; i < Rows; i++) {
      result(i, i) = T{1};
    }
    return result;
  }

  /**
   * @brief Transpose the matrix.
   * @return Transposed matrix.
   */
  Matrix<Cols, Rows, T> transpose() const noexcept {
    Matrix<Cols, Rows, T> result;
    for (std::size_t i = 0; i < Rows; i++) {
      for (std::size_t j = 0; j < Cols; j++) {
        result(j, i) = m_matrix[i][j];
      }
    }
    return result;
  }

  /**
   * @brief Get the matrix data.
   * @return Const reference to the matrix data.
   */
  const std::array<std::array<T, Cols>, Rows> &data() const noexcept {
    return m_matrix;
  }

  /**
   * @brief Get the matrix data (mutable).
   * @return Reference to the matrix data.
   */
  std::array<std::array<T, Cols>, Rows> &data() noexcept { return m_matrix; }

  /**
   * @brief Calculate the determinant of the matrix.
   * @return Determinant value.
   */
  T determinant() const noexcept {
    static_assert(Rows == Cols, "Determinant only defined for square matrices");

    if constexpr (Rows == 1) {
      return m_matrix[0][0];
    } else if constexpr (Rows == 2) {
      return m_matrix[0][0] * m_matrix[1][1] - m_matrix[0][1] * m_matrix[1][0];
    } else if constexpr (Rows == 3) {
      return m_matrix[0][0] * (m_matrix[1][1] * m_matrix[2][2] -
                               m_matrix[1][2] * m_matrix[2][1]) -
             m_matrix[0][1] * (m_matrix[1][0] * m_matrix[2][2] -
                               m_matrix[1][2] * m_matrix[2][0]) +
             m_matrix[0][2] * (m_matrix[1][0] * m_matrix[2][1] -
                               m_matrix[1][1] * m_matrix[2][0]);
    } else if constexpr (Rows == 4) {
      return m_matrix[0][0] * cofactor(0, 0) + m_matrix[0][1] * cofactor(0, 1) +
             m_matrix[0][2] * cofactor(0, 2) + m_matrix[0][3] * cofactor(0, 3);
    } else {
      // For other sizes we need to implement a better algorithm
      return T{};
    }
  }

  /**
   * @brief Calculate the cofactor for element (i,j).
   * @param i Row index.
   * @param j Column index.
   * @return Cofactor value.
   */
  T cofactor(std::size_t i, std::size_t j) const noexcept {
    static_assert(Rows == Cols, "Cofactor only defined for square matrices");
    T minor = minorSubmatrix(i, j).determinant();
    return ((i + j) % 2 == 0) ? minor : -minor;
  }

  /**
   * @brief Get the minor submatrix by removing row i and column j.
   * @param i Row to remove.
   * @param j Column to remove.
   * @return Submatrix.
   */
  Matrix<Rows - 1, Cols - 1, T> minorSubmatrix(std::size_t i,
                                               std::size_t j) const noexcept {
    static_assert(Rows > 1 && Cols > 1,
                  "Submatrix requires at least 2x2 matrix");

    Matrix<Rows - 1, Cols - 1, T> result;
    std::size_t r = 0;
    for (std::size_t row = 0; row < Rows; row++) {
      if (row == i) {
        continue;
      }
      std::size_t c = 0;
      for (std::size_t col = 0; col < Cols; col++) {
        if (col == j) {
          continue;
        }
        result(r, c) = m_matrix[row][col];
        c++;
      }
      r++;
    }
    return result;
  }

  /**
   * @brief Calculate the inverse of a 4x4 matrix.
   * @return Inverse matrix.
   * @note This is an optimized version for 4x4 transformation matrices.
   */
  Matrix inverse() const noexcept {
    static_assert(Rows == Cols, "Inverse only defined for square matrices");

    if constexpr (Rows == 4) {
      T det = determinant();
      if (std::abs(det) < 1e-8) {
        return identity();
      }
      Matrix result;
      for (std::size_t i = 0; i < 4; i++) {
        for (std::size_t j = 0; j < 4; j++) {
          result(j, i) = cofactor(i, j) / det;
        }
      }
      return result;
    } else {
      // For other sizes we need to implement a general inverse algorithm
      return *this;
    }
  }

private:
  std::array<std::array<T, Cols>, Rows> m_matrix;
};

/**
 * @brief Multiply a scalar by a matrix.
 * @param scalar The scalar value.
 * @param matrix The matrix to multiply.
 * @return New matrix representing the product.
 */
template <std::size_t Rows, std::size_t Cols, typename T>
Matrix<Rows, Cols, T> operator*(const T scalar,
                                const Matrix<Rows, Cols, T> &matrix) noexcept {
  return matrix * scalar;
}

using Matrix4 = Matrix<4, 4, double>;

/**
 * @brief Create a translation matrix.
 * @param tx X translation.
 * @param ty Y translation.
 * @param tz Z translation.
 * @return Translation matrix.
 */
inline Matrix4 translate(double tx, double ty, double tz) noexcept {
  Matrix4 result = Matrix4::identity();
  result(0, 3) = tx;
  result(1, 3) = ty;
  result(2, 3) = tz;
  return result;
}

/**
 * @brief Transform a 3D point using a 4x4 transformation matrix.
 * @param matrix The transformation matrix.
 * @param point The point to transform.
 * @return Transformed point.
 */
inline Point<3> transformPoint(const Matrix4 &matrix,
                               const Point<3> &point) noexcept {
  double x = matrix(0, 0) * point.m_components[0] +
             matrix(0, 1) * point.m_components[1] +
             matrix(0, 2) * point.m_components[2] + matrix(0, 3);
  double y = matrix(1, 0) * point.m_components[0] +
             matrix(1, 1) * point.m_components[1] +
             matrix(1, 2) * point.m_components[2] + matrix(1, 3);
  double z = matrix(2, 0) * point.m_components[0] +
             matrix(2, 1) * point.m_components[1] +
             matrix(2, 2) * point.m_components[2] + matrix(2, 3);
  return Point<3>(x, y, z);
}

/**
 * @brief Transform a 3D vector using a 4x4 transformation matrix.
 * @param matrix The transformation matrix.
 * @param vector The vector to transform.
 * @return Transformed vector.
 */
inline Vector<3> transformVector(const Matrix4 &matrix,
                                 const Vector<3> &vector) noexcept {
  double x = matrix(0, 0) * vector.m_components[0] +
             matrix(0, 1) * vector.m_components[1] +
             matrix(0, 2) * vector.m_components[2];
  double y = matrix(1, 0) * vector.m_components[0] +
             matrix(1, 1) * vector.m_components[1] +
             matrix(1, 2) * vector.m_components[2];
  double z = matrix(2, 0) * vector.m_components[0] +
             matrix(2, 1) * vector.m_components[1] +
             matrix(2, 2) * vector.m_components[2];
  return Vector<3>(x, y, z);
}

/**
 * @brief Transform a normal vector using the inverse transpose of a matrix.
 * @param matrix The transformation matrix.
 * @param normal The normal vector to transform.
 * @return Transformed normal vector.
 */
inline Vector<3> transformNormal(const Matrix4 &matrix,
                                 const Vector<3> &normal) noexcept {
  double x = matrix(0, 0) * normal.m_components[0] +
             matrix(1, 0) * normal.m_components[1] +
             matrix(2, 0) * normal.m_components[2];
  double y = matrix(0, 1) * normal.m_components[0] +
             matrix(1, 1) * normal.m_components[1] +
             matrix(2, 1) * normal.m_components[2];
  double z = matrix(0, 2) * normal.m_components[0] +
             matrix(1, 2) * normal.m_components[1] +
             matrix(2, 2) * normal.m_components[2];
  return Vector<3>(x, y, z).normalize();
}

} // namespace Raytracer::Math