cmake_minimum_required(VERSION 3.20)
project(raytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
  src/main.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(raytracer ${SOURCES})

target_include_directories(raytracer
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(raytracer
  PRIVATE
    config++
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
    criterion
)

target_compile_options(raytracer_tests
  PRIVATE
    -Wall -Wextra -Werror
)

add_test(
  NAME raytracer_tests
  COMMAND raytracer_tests
)
