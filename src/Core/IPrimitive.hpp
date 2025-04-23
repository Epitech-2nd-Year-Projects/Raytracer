#pragma once

#include <memory>
#include <optional>

#include "BoundingBox.hpp"
#include "IMaterial.hpp"
#include "Intersection.hpp"
#include "Math/Point.hpp"
#include "Math/Vector.hpp"
#include "Ray.hpp"

namespace Raytracer::Core {

class IPrimitive {

public:
  virtual ~IPrimitive() = default;

  [[nodiscard]] virtual std::optional<Intersection>
  intersect(const Ray &ray) const noexcept = 0;

  [[nodiscard]] virtual BoundingBox getBoundingBox() const noexcept = 0;
};
} // namespace Raytracer::Core