#pragma once

#include "Core/Ray.hpp"
#include "Math/Point.hpp"

namespace Raytracer::Core {

class BoundingBox final {
public:
  constexpr BoundingBox() noexcept = default;
  constexpr BoundingBox(const Math::Point<3> &min,
                        const Math::Point<3> &max) noexcept;

  [[nodiscard]] bool intersect(const Ray &ray) const noexcept;
  [[nodiscard]] BoundingBox unite(const BoundingBox &other) const noexcept;
  [[nodiscard]] constexpr bool
  contains(const Math::Point<3> &point) const noexcept;

  [[nodiscard]] constexpr Math::Point<3> &getMin() const noexcept;
  [[nodiscard]] constexpr Math::Point<3> &getMax() const noexcept;

private:
  Math::Point<3> m_min{};
  Math::Point<3> m_max{};
};
} // namespace Raytracer::Core
