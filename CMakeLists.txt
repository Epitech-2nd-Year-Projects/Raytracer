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
