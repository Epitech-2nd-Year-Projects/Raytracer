cmake_minimum_required(VERSION 3.20)
project(raytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)
if (TARGET SFML::Graphics)
  set(_SFML_LIBS SFML::Graphics SFML::Window SFML::System)
else()
  set(_SFML_LIBS
    ${SFML_GRAPHICS_LIBRARY}
    ${SFML_WINDOW_LIBRARY}
    ${SFML_SYSTEM_LIBRARY}
    ${SFML_DEPENDENCIES}
  )
  set(_SFML_INCLUDES ${SFML_INCLUDE_DIR})
endif()

find_path(LIBCONFIGPP_INCLUDE_DIR
  NAMES libconfig.h++
  HINTS /usr/include /usr/local/include
)

find_library(LIBCONFIGPP_LIBRARY
  NAMES config++ libconfig++
  HINTS /usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib
)

set(SOURCES
  src/main.cpp
  src/Core/Ray.cpp
  src/Core/Intersection.cpp
  src/Core/Color.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(raytracer ${SOURCES})

target_include_directories(raytracer
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${LIBCONFIGPP_INCLUDE_DIR}
    ${_SFML_INCLUDES}
)

target_link_libraries(raytracer
  PRIVATE
    ${LIBCONFIGPP_LIBRARY}
    ${_SFML_LIBS}
)

target_compile_options(raytracer
  PRIVATE
    -Wall -Wextra -Werror
)

enable_testing()

find_package(PkgConfig REQUIRED)
pkg_check_modules(CRITERION REQUIRED criterion)

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
    ${CRITERION_INCLUDE_DIRS}
)

target_link_libraries(raytracer_tests
  PRIVATE
    ${CRITERION_LIBRARIES}
)

target_compile_options(raytracer_tests
  PRIVATE
    -Wall -Wextra -Werror
)

add_test(
  NAME raytracer_tests
  COMMAND raytracer_tests
)
