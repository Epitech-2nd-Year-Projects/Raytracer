/**
 * @file Transform.hpp
 * @brief Defines the Transform class for handling spatial transformations.
 */
#pragma once

#include "Core/BoundingBox.hpp"
#include "Core/Ray.hpp"
#include "Math/Matrix.hpp"
#include "Math/Point.hpp"
#include "Math/Vector.hpp"

namespace Raytracer::Math {

/**
 * @class Transform
 * @brief Represents a spatial transformation with matrix and its inverse.
 */
class Transform {
public:
  /**
   * @brief Default constructor creates an identity transform.
   */
  Transform() noexcept
      : m_matrix(Matrix4::identity()), m_inverse(Matrix4::identity()) {}

  /**
   * @brief Construct from a transformation matrix.
   * @param matrix The transformation matrix.
   */
  explicit Transform(const Matrix4 &matrix) noexcept
      : m_matrix(matrix), m_inverse(matrix.inverse()) {}

  /**
   * @brief Construct from a transformation matrix and its precomputed inverse.
   * @param matrix The transformation matrix.
   * @param inverse The inverse transformation matrix.
   */
  Transform(const Matrix4 &matrix, const Matrix4 &inverse) noexcept
      : m_matrix(matrix), m_inverse(inverse) {}

  /**
   * @brief Create a translation transform.
   * @param tx X translation.
   * @param ty Y translation.
   * @param tz Z translation.
   * @return Translation transform.
   */
  static Transform translate(double tx, double ty, double tz) noexcept {
    Matrix4 matrix = Matrix4::identity();
    matrix(0, 3) = tx;
    matrix(1, 3) = ty;
    matrix(2, 3) = tz;

    Matrix4 inverse = Matrix4::identity();
    inverse(0, 3) = -tx;
    inverse(1, 3) = -ty;
    inverse(2, 3) = -tz;
    return Transform(matrix, inverse);
  }

  /**
   * @brief Create a translation transform.
   * @param translation Translation vector.
   * @return Translation transform.
   */
  static Transform translate(const Vector<3> &translation) noexcept {
    return translate(translation.m_components[0], translation.m_components[1],
                     translation.m_components[2]);
  }

  /**
   * @brief Get the transformation matrix.
   * @return The transformation matrix.
   */
  [[nodiscard]] const Matrix4 &getMatrix() const noexcept { return m_matrix; }

  /**
   * @brief Get the inverse transformation matrix.
   * @return The inverse transformation matrix.
   */
  [[nodiscard]] const Matrix4 &getInverse() const noexcept { return m_inverse; }

  /**
   * @brief Get the inverse transform.
   * @return The inverse transform.
   */
  [[nodiscard]] Transform inverse() const noexcept {
    return Transform(m_inverse, m_matrix);
  }

  /**
   * @brief Combine this transform with another.
   * @param other The transform to apply after this one.
   * @return Combined transform.
   */
  [[nodiscard]] Transform combine(const Transform &other) const noexcept {
    return Transform(other.m_matrix.multiply(m_matrix),
                     m_inverse.multiply(other.m_inverse));
  }

  /**
   * @brief Operator for combining transforms.
   * @param other The transform to apply after this one.
   * @return Combined transform.
   */
  [[nodiscard]] Transform operator*(const Transform &other) const noexcept {
    return combine(other);
  }

  /**
   * @brief Transform a point.
   * @param point Point to transform.
   * @return Transformed point.
   */
  [[nodiscard]] Point<3> transformPoint(const Point<3> &point) const noexcept {
    return Math::transformPoint(m_matrix, point);
  }

  /**
   * @brief Transform a vector.
   * @param vector Vector to transform.
   * @return Transformed vector.
   */
  [[nodiscard]] Vector<3>
  transformVector(const Vector<3> &vector) const noexcept {
    return Math::transformVector(m_matrix, vector);
  }

  /**
   * @brief Transform a normal vector.
   * @param normal Normal vector to transform.
   * @return Transformed normal vector.
   */
  [[nodiscard]] Vector<3>
  transformNormal(const Vector<3> &normal) const noexcept {
    return Math::transformNormal(m_matrix, normal);
  }

  /**
   * @brief Transform a ray.
   * @param ray Ray to transform.
   * @return Transformed ray.
   */
  [[nodiscard]] Core::Ray transformRay(const Core::Ray &ray) const noexcept {
    Point<3> origin = transformPoint(ray.getOrigin());
    Vector<3> direction = transformVector(ray.getDirection());
    return Core::Ray(origin, direction, ray.getMinDistance(),
                     ray.getMaxDistance());
  }

  /**
   * @brief Transform a ray using the inverse transform.
   * @param ray Ray to transform.
   * @return Transformed ray.
   */
  [[nodiscard]] Core::Ray
  inverseTransformRay(const Core::Ray &ray) const noexcept {
    Point<3> origin = Math::transformPoint(m_inverse, ray.getOrigin());
    Vector<3> direction = Math::transformVector(m_inverse, ray.getDirection());
    return Core::Ray(origin, direction, ray.getMinDistance(),
                     ray.getMaxDistance());
  }

  /**
   * @brief Transform a bounding box.
   * @param box Bounding box to transform.
   * @return Transformed bounding box.
   */
  [[nodiscard]] Core::BoundingBox
  transformBoundingBox(const Core::BoundingBox &box) const noexcept {
    const Point<3> &minPoint = box.getMin();
    const Point<3> &maxPoint = box.getMax();

    Point<3> corner = transformPoint(minPoint);
    Point<3> newMin = corner;
    Point<3> newMax = corner;
    Point<3> corners[7] = {
        Point<3>(maxPoint.m_components[0], minPoint.m_components[1],
                 minPoint.m_components[2]),
        Point<3>(minPoint.m_components[0], maxPoint.m_components[1],
                 minPoint.m_components[2]),
        Point<3>(maxPoint.m_components[0], maxPoint.m_components[1],
                 minPoint.m_components[2]),
        Point<3>(minPoint.m_components[0], minPoint.m_components[1],
                 maxPoint.m_components[2]),
        Point<3>(maxPoint.m_components[0], minPoint.m_components[1],
                 maxPoint.m_components[2]),
        Point<3>(minPoint.m_components[0], maxPoint.m_components[1],
                 maxPoint.m_components[2]),
        Point<3>(maxPoint.m_components[0], maxPoint.m_components[1],
                 maxPoint.m_components[2])};

    for (const auto &c : corners) {
      Point<3> transformed = transformPoint(c);
      for (size_t i = 0; i < 3; ++i) {
        newMin.m_components[i] =
            std::min(newMin.m_components[i], transformed.m_components[i]);
        newMax.m_components[i] =
            std::max(newMax.m_components[i], transformed.m_components[i]);
      }
    }
    return Core::BoundingBox(newMin, newMax);
  }

private:
  Matrix4 m_matrix;
  Matrix4 m_inverse;
};

} // namespace Raytracer::Math