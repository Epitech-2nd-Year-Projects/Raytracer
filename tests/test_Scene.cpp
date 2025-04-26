/**
 * @file test_Scene.cpp
 * @brief Unit tests for the Scene class.
 */

#include "../src/Core/ALight.hpp"
#include "../src/Core/APrimitive.hpp"
#include "../src/Core/BoundingBox.hpp"
#include "../src/Core/Camera.hpp"
#include "../src/Core/Intersection.hpp"
#include "../src/Core/Ray.hpp"
#include "../src/Core/Scene.hpp"
#include "../src/Math/Point.hpp"
#include "../src/Math/Rectangle.hpp"
#include "../src/Math/Vector.hpp"
#include "Core/IPrimitive.hpp"
#include <array>
#include <criterion/criterion.h>
#include <memory>
#include <optional>
#include <string>

static constexpr double EQ_APPROX = 1e-9;

using namespace Raytracer::Core;
using Raytracer::Math::Point;
using Raytracer::Math::Rectangle3D;
using Raytracer::Math::Vector;

namespace Raytracer::Core {
class ILight;
}

class MockPrimitive : public Raytracer::Core::APrimitive {
public:
  MockPrimitive() = default;
  ~MockPrimitive() override = default;

  [[nodiscard]] std::optional<Raytracer::Core::Intersection>
  intersect(const Raytracer::Core::Ray &) const noexcept override {
    return std::nullopt;
  }

  [[nodiscard]] Raytracer::Core::BoundingBox
  getBoundingBox() const noexcept override {
    return Raytracer::Core::BoundingBox();
  }
};

class MockDirectionalLight : public Raytracer::Core::ADirectionalLight {
public:
  using Raytracer::Core::ADirectionalLight::ADirectionalLight;

  [[nodiscard]] bool castsShadow() const noexcept override { return true; }
};

template <std::size_t N>
void assert_point_eq(const Raytracer::Math::Point<N> &p,
                     const std::array<double, N> &ref, double eps = EQ_APPROX) {
  for (std::size_t i = 0; i < N; ++i) {
    cr_assert_float_eq(p.m_components[i], ref[i], eps,
                       "Point component[%zu]: expected %g but got %g", i,
                       ref[i], p.m_components[i]);
  }
}

template <std::size_t N>
void assert_vec_eq(const Raytracer::Math::Vector<N> &v,
                   const std::array<double, N> &ref, double eps = EQ_APPROX) {
  for (std::size_t i = 0; i < N; ++i) {
    cr_assert_float_eq(v.m_components[i], ref[i], eps,
                       "Vector component[%zu]: expected %g but got %g", i,
                       ref[i], v.m_components[i]);
  }
}

TestSuite(SceneSuite);

Test(SceneSuite, DefaultConstructor) {
  Scene scene;

  cr_assert(scene.getPrimitives().empty(),
            "Default scene should have no primitives.");
  cr_assert(scene.getLights().empty(), "Default scene should have no lights.");

  const Camera &cam = scene.getCamera();

  assert_point_eq(cam.getOrigin(), {0.0, 0.0, 0.0});
}

Test(SceneSuite, AddAndGetPrimitive) {
  Scene scene;
  std::string id1 = "sphere1";
  auto primitive1 = std::make_unique<MockPrimitive>();

  IPrimitive *primitive1RawPtr = primitive1.get();
  cr_assert_not_null(primitive1RawPtr,
                     "Raw pointer before move should be valid.");

  cr_assert(scene.addPrimitive(id1, std::move(primitive1)),
            "Failed to add first primitive.");
  cr_assert_null(primitive1.get(),
                 "Original unique_ptr should be null after move.");
  cr_assert_eq(scene.getPrimitives().size(), 1,
               "Primitive count should be 1 after add.");

  IPrimitive *retrievedPrimitivePtr = scene.getPrimitive(id1);

  cr_assert_not_null(retrievedPrimitivePtr,
                     "Failed to retrieve existing primitive pointer.");
  cr_assert_eq(retrievedPrimitivePtr, primitive1RawPtr,
               "Retrieved pointer does not match the original object.");

  const Scene &const_scene = scene;
  const IPrimitive *retrievedConstPrimitivePtr = const_scene.getPrimitive(id1);

  cr_assert_not_null(retrievedConstPrimitivePtr,
                     "Failed to retrieve const primitive pointer.");
  cr_assert_eq(retrievedConstPrimitivePtr, primitive1RawPtr,
               "Retrieved const pointer does not match the original object.");

  cr_assert_null(scene.getPrimitive("nonexistent"),
                 "Retrieving non-existent primitive should return nullptr.");
  cr_assert_null(
      const_scene.getPrimitive("nonexistent"),
      "Retrieving non-existent const primitive should return nullptr.");

  auto primitive2 = std::make_unique<MockPrimitive>();

  cr_assert_not(
      scene.addPrimitive(id1, std::move(primitive2)),
      "Adding primitive with duplicate ID should fail (return false).");
  cr_assert_eq(
      scene.getPrimitives().size(), 1,
      "Primitive count should still be 1 after duplicate add attempt.");
  cr_assert_eq(scene.getPrimitive(id1), primitive1RawPtr,
               "Original primitive should remain after duplicate add attempt.");
}

Test(SceneSuite, RemovePrimitive) {
  Scene scene;
  std::string id1 = "p1";
  auto primitive1 = std::make_unique<MockPrimitive>();
  IPrimitive *primitive1RawPtr = primitive1.get();

  scene.addPrimitive(id1, std::move(primitive1));

  cr_assert_eq(scene.getPrimitives().size(), 1);
  cr_assert_eq(scene.getPrimitive(id1), primitive1RawPtr,
               "Primitive should exist before removal.");
  cr_assert(scene.removePrimitive(id1),
            "Failed to remove existing primitive (should return true).");
  cr_assert_eq(scene.getPrimitives().size(), 0,
               "Primitive count should be 0 after removal.");
  cr_assert_null(scene.getPrimitive(id1),
                 "Primitive pointer should be null after removal.");
  cr_assert_not(
      scene.removePrimitive(id1),
      "Removing already removed primitive should fail (return false).");
  cr_assert_eq(scene.getPrimitives().size(), 0,
               "Primitive count should still be 0.");
  cr_assert_not(scene.removePrimitive("nonexistent"),
                "Removing non-existent primitive should fail (return false).");
  cr_assert(scene.getPrimitives().empty(),
            "Primitive map should remain empty.");
}

Test(SceneSuite, AddAndGetLight) {
  Scene scene;
  std::string id1 = "light_d";
  auto light1 = std::make_unique<MockDirectionalLight>();
  ILight *light1RawPtr = light1.get();

  cr_assert(scene.addLight(id1, std::move(light1)), "Failed to add light.");
  cr_assert_null(light1.get(),
                 "Original light unique_ptr should be null after move.");
  cr_assert_eq(scene.getLights().size(), 1, "Light count should be 1.");

  ILight *retrievedLightPtr = scene.getLight(id1);

  cr_assert_not_null(retrievedLightPtr, "Failed to get light pointer.");
  cr_assert_eq(retrievedLightPtr, light1RawPtr,
               "Retrieved light pointer mismatch.");

  const Scene &const_scene = scene;
  const ILight *retrievedConstLightPtr = const_scene.getLight(id1);

  cr_assert_not_null(retrievedConstLightPtr,
                     "Failed to get const light pointer.");
  cr_assert_eq(retrievedConstLightPtr, light1RawPtr,
               "Retrieved const light pointer mismatch.");

  cr_assert_null(scene.getLight("nonexistent"));
  cr_assert_null(const_scene.getLight("nonexistent"));

  auto light2 = std::make_unique<MockDirectionalLight>();

  cr_assert_not(scene.addLight(id1, std::move(light2)),
                "Duplicate light add should fail.");
  cr_assert_eq(scene.getLights().size(), 1, "Light count should remain 1.");
  cr_assert_eq(scene.getLight(id1), light1RawPtr,
               "Original light should remain.");
}

Test(SceneSuite, RemoveLight) {
  Scene scene;
  std::string id1 = "l1";
  auto light1 = std::make_unique<MockDirectionalLight>();
  ILight *light1RawPtr = light1.get();

  scene.addLight(id1, std::move(light1));

  cr_assert_eq(scene.getLights().size(), 1);
  cr_assert_eq(scene.getLight(id1), light1RawPtr);
  cr_assert(scene.removeLight(id1), "Remove existing light should succeed.");
  cr_assert_eq(scene.getLights().size(), 0, "Light count should be 0.");
  cr_assert_null(scene.getLight(id1),
                 "Getting removed light should yield nullptr.");
  cr_assert_not(scene.removeLight(id1),
                "Removing already removed should fail.");
  cr_assert_not(scene.removeLight("nonexistent"),
                "Removing non-existent should fail.");
  cr_assert(scene.getLights().empty());
}

Test(SceneSuite, SetAndGetCamera) {
  Scene scene;
  Camera defaultCamera = scene.getCamera();
  Point<3> newOrigin(10.0, 20.0, -5.0);
  Rectangle3D newScreen(Point<3>(0.0, 0.0, 1.0), Vector<3>(2.0, 0.0, 0.0),
                        Vector<3>(0.0, 1.5, .0));
  double newFov = 60.0 * M_PI / 180.0;
  Camera newCamera(newOrigin, newScreen, newFov);

  scene.setCamera(newCamera);

  const Camera &retrievedCamConst = scene.getCamera();

  assert_point_eq(retrievedCamConst.getOrigin(), {10.0, 20.0, -5.0});
  cr_assert_float_eq(retrievedCamConst.getFov(), newFov, EQ_APPROX);

  Camera &retrievedCamMut = scene.getCamera();
  Point<3> modifiedOrigin(1.0, 1.0, 1.0);
  retrievedCamMut.setOrigin(modifiedOrigin);
  retrievedCamMut.setFov(45.0 * M_PI / 180.0);

  const Camera &finalCam = scene.getCamera();

  assert_point_eq(finalCam.getOrigin(), {1.0, 1.0, 1.0});
  cr_assert_float_eq(finalCam.getFov(), 45.0 * M_PI / 180.0, EQ_APPROX);
}

Test(SceneSuite, ClearMethods) {
  Scene scene;
  auto primitive1 = std::make_unique<MockPrimitive>();
  auto light1 = std::make_unique<MockDirectionalLight>();

  scene.addPrimitive("p1", std::move(primitive1));
  scene.addLight("l1", std::move(light1));

  cr_assert_eq(scene.getPrimitives().size(), 1);
  cr_assert_eq(scene.getLights().size(), 1);
  scene.clearPrimitives();
  cr_assert(scene.getPrimitives().empty(),
            "Primitives should be empty after clearPrimitives.");
  cr_assert_eq(scene.getLights().size(), 1,
               "Lights should remain after clearPrimitives.");
  scene.addPrimitive("p2", std::make_unique<MockPrimitive>());
  cr_assert_eq(scene.getPrimitives().size(), 1);
  scene.clearLights();
  cr_assert_eq(scene.getPrimitives().size(), 1,
               "Primitives should remain after clearLights.");
  cr_assert(scene.getLights().empty(),
            "Lights should be empty after clearLights.");
  scene.addLight("l2", std::make_unique<MockDirectionalLight>());
  cr_assert_eq(scene.getPrimitives().size(), 1);
  cr_assert_eq(scene.getLights().size(), 1);
  scene.clear();
  cr_assert(scene.getPrimitives().empty(),
            "Primitives should be empty after clear.");
  cr_assert(scene.getLights().empty(), "Lights should be empty after clear.");
}

Test(SceneSuite, GetContainers) {
  Scene scene;
  std::string primitiveId = "the_primitive";
  std::string lightId = "the_light";
  auto primitive1 = std::make_unique<MockPrimitive>();
  IPrimitive *primitive1RawPtr = primitive1.get();
  auto light1 = std::make_unique<MockDirectionalLight>();
  ILight *light1RawPtr = light1.get();

  scene.addPrimitive(primitiveId, std::move(primitive1));
  scene.addLight(lightId, std::move(light1));

  const auto &primitives = scene.getPrimitives();
  const auto &lights = scene.getLights();

  cr_assert_eq(primitives.size(), 1);
  cr_assert_eq(lights.size(), 1);

  auto primitiveIt = primitives.find(primitiveId);

  cr_assert(primitiveIt != primitives.end(),
            "Primitive map should contain added primitive ID.");
  cr_assert_eq(primitiveIt->second.get(), primitive1RawPtr,
               "Primitive map iterator should point to the correct primitive.");

  auto lightIt = lights.find(lightId);

  cr_assert(lightIt != lights.end(),
            "Light map should contain added light ID.");
  cr_assert_eq(lightIt->second.get(), light1RawPtr,
               "Light map iterator should point to the correct light.");

  cr_assert(primitives.find("non_existent_prim") == primitives.end());
  cr_assert(lights.find("non_existent_light") == lights.end());
}
