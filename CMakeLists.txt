cmake_minimum_required(VERSION 3.20)
project(raytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBConfig++ REQUIRED IMPORTED_TARGET libconfig++)
pkg_check_modules(Criterion REQUIRED IMPORTED_TARGET criterion)

set(CORE_SOURCES
  src/Core/Renderer.cpp
  src/Builder/SceneBuilder.cpp
  src/Parser/SceneParser.cpp
  src/Core/Scene.cpp
  src/Plugin/PluginManager.cpp
)

add_library(raytracer_core STATIC ${CORE_SOURCES})
target_include_directories(raytracer_core PUBLIC ${CMAKE_SOURCE_DIR}/src)
set_property(TARGET raytracer_core PROPERTY POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

function(add_raytracer_plugin PLUGIN_NAME PLUGIN_SOURCES)
    add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES})
    target_include_directories(${PLUGIN_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

    set_target_properties(${PLUGIN_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/plugins"
    )
    target_link_libraries(${PLUGIN_NAME} PRIVATE raytracer_core)
endfunction()

add_executable(raytracer src/main.cpp)

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
    raytracer_core
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
  tests/test_Rectangle.cpp
  tests/test_Camera.cpp
  tests/test_Scene.cpp
  tests/test_Renderer.cpp
  tests/test_SceneParser.cpp
  tests/test_Matrix.cpp
  tests/test_Transform.cpp
  tests/test_Rotation.cpp
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
    PkgConfig::LIBConfig++
    raytracer_core
)

target_compile_options(raytracer_tests
  PRIVATE
    -Wall -Wextra -Werror
)

add_test(
  NAME raytracer_tests
  COMMAND raytracer_tests
)

add_raytracer_plugin(sphere_plugin plugins/SpherePlugin.cpp)
add_raytracer_plugin(plane_plugin plugins/PlanePlugin.cpp)
add_raytracer_plugin(cylinder_plugin plugins/CylinderPlugin.cpp)
add_raytracer_plugin(cone_plugin plugins/ConePlugin.cpp)
add_raytracer_plugin(flat_material_plugin plugins/FlatMaterialPlugin.cpp)
add_raytracer_plugin(mirror_material_plugin plugins/MirrorMaterialPlugin.cpp)
add_raytracer_plugin(steel_material_plugin plugins/SteelMaterialPlugin.cpp)
add_raytracer_plugin(point_light_plugin plugins/PointLightPlugin.cpp)
add_raytracer_plugin(ambient_light_plugin plugins/AmbientLightPlugin.cpp)
add_raytracer_plugin(diffuse_light_plugin plugins/DiffuseLightPlugin.cpp)
