#pragma once

#include <memory>

#include "Math/Point.hpp"
#include "Math/Vector.hpp"

namespace Raytracer::Core {
class IMaterial;

class Intersection final {
public:
  constexpr Intersection() noexcept = default;
  Intersection(const Math::Point<3> &point, const Math::Vector<3> &normal,
               std::unique_ptr<IMaterial> material, double distance,
               bool isInside, const Math::Point<2> &uv) noexcept;

  [[nodiscard]] constexpr const Math::Point<3> &getPoint() const noexcept;
  [[nodiscard]] constexpr const Math::Vector<3> &getNormal() const noexcept;
  [[nodiscard]] std::unique_ptr<IMaterial> getMaterial() const noexcept;
  [[nodiscard]] constexpr double getDistance() const noexcept;
  [[nodiscard]] constexpr bool getIsInside() const noexcept;
  [[nodiscard]] constexpr const Math::Point<2> &getUv() const noexcept;

private:
  Math::Point<3> m_point{};
  Math::Vector<3> m_normal{};
  std::unique_ptr<IMaterial> m_material{nullptr};
  double m_distance{0.0};
  bool m_isInside{false};
  Math::Point<2> m_uv{0.0, 0.0};
};
} // namespace Raytracer::Core
