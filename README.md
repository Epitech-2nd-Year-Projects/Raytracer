# Raytracer

A simple CPU‐based raytracer written in in C++, using SFML for windowing/graphics and libconfig++ for configuration.

## Build

```bash
mkdir build && cd build

cmake ..

cmake --build .
```

The `raytracer` executable will be placed in the root directory.

## Tests

We use Criterion + CTest to run unit tests:

```bash
ctest --output-on-failure
```

The test binary is named `raytracer_tests`.

### Documentation

We use Doxygen to generate API documentation. A Doxyfile is provided at the project root.

```bash
doxygen Doxyfile
```

HTML docs will be available under `docs/doxygen/html/index.html`.
