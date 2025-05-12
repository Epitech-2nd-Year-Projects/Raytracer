#!/bin/bash

if [ ! -f "./raytracer" ]; then
    echo "Error: raytracer binary not found. Please build the project first."
    exit 1
fi

echo "=== Raytracer Performance Benchmark ==="
echo "Testing with and without multithreading..."
echo ""

SCENES=("scenes/base.scene" "scenes/steel.scene" "scenes/cone.scene")

for scene in "${SCENES[@]}"; do
    echo "=== Testing $scene ==="

    echo "Running single-threaded..."
    TIMEFORMAT="Single-threaded time: %3Rs"
    time ./raytracer "$scene" -m -o "output_st.ppm" > /dev/null

    echo "Running multi-threaded..."
    TIMEFORMAT="Multi-threaded time:  %3Rs"
    time ./raytracer "$scene" -o "output_mt.ppm" > /dev/null

    echo "========================================"
    echo ""
done

echo "Benchmark completed!"
echo "To see more accurate results, try rendering higher resolution images or more complex scenes."