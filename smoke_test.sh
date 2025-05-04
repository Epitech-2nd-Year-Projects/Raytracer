#!/bin/bash

# Smoke test script for raytracer
# Renders all scene files and checks if the process completed successfully

OUTPUT_DIR="./test_output"
SCENE_DIR="./scenes"
EXECUTABLE="./raytracer"

mkdir -p "$OUTPUT_DIR"

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m'

echo "=== Raytracer Smoke Test ==="
echo "Rendering all scene files from $SCENE_DIR to $OUTPUT_DIR"

TOTAL=0
PASSED=0
FAILED=0

SCENE_FILES=$(find "$SCENE_DIR" -name "*.scene")

for SCENE_FILE in $SCENE_FILES; do
    TOTAL=$((TOTAL+1))
    BASENAME=$(basename "$SCENE_FILE" .scene)
    OUTPUT_FILE="$OUTPUT_DIR/${BASENAME}.ppm"
    
    echo -e "\n${YELLOW}Testing:${NC} $BASENAME"
    echo "Input: $SCENE_FILE"
    echo "Output: $OUTPUT_FILE"
    
    "$EXECUTABLE" "$SCENE_FILE" > /dev/null 2>&1
    EXIT_CODE=$?
    
    if [ -f "output.ppm" ]; then
        mv output.ppm "$OUTPUT_FILE"
    fi
    
    if [ $EXIT_CODE -eq 0 ] && [ -f "$OUTPUT_FILE" ]; then
        echo -e "${GREEN}✓ Passed${NC}"
        PASSED=$((PASSED+1))
    else
        echo -e "${RED}✗ Failed${NC} (Exit code: $EXIT_CODE)"
        FAILED=$((FAILED+1))
    fi
done

echo -e "\n=== Test Summary ==="
echo "Total: $TOTAL"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}Some tests failed!${NC}"
    exit 1
fi
