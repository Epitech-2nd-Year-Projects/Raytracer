cmake_minimum_required(VERSION 3.20)
project(raytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBConfig++ REQUIRED IMPORTED_TARGET libconfig++)
pkg_check_modules(Criterion REQUIRED IMPORTED_TARGET criterion)

set(SOURCES
  src/main.cpp
  src/Primitives/Sphere.cpp
  src/Core/Renderer.cpp
  src/Materials/FlatMaterial.cpp
  src/Primitives/Plane.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(raytracer ${SOURCES})

target_include_directories(raytracer
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer
  PRIVATE
    PkgConfig::LIBConfig++
    sfml-graphics
    sfml-window
    sfml-system
)

target_compile_options(raytracer
  PRIVATE
    -Wall -Wextra -Werror
)

enable_testing()

set(TEST_SOURCES
  tests/test_Vector.cpp
  tests/test_Point.cpp
  tests/test_Intersection.cpp
  tests/test_Ray.cpp
  tests/test_Sphere.cpp
  tests/test_Rectangle.cpp
  tests/test_Camera.cpp
  tests/test_FlatMaterial.cpp
  tests/test_Scene.cpp
  tests/test_Renderer.cpp
  tests/test_AmbientLight.cpp
)

set(IMPLEMENTATION_SOURCES ${SOURCES})
list(REMOVE_ITEM IMPLEMENTATION_SOURCES src/main.cpp)

add_executable(raytracer_tests
  ${TEST_SOURCES}
  ${IMPLEMENTATION_SOURCES}
)

target_include_directories(raytracer_tests
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer_tests
  PRIVATE
    PkgConfig::Criterion
)

target_compile_options(raytracer_tests
  PRIVATE
    -Wall -Wextra -Werror
)

add_test(
  NAME raytracer_tests
  COMMAND raytracer_tests
)
