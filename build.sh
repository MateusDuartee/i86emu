#!/bin/bash
set -e

BUILD_DIR="build"

mkdir -p "$BUILD_DIR"

if command -v g++ >/dev/null 2>&1; then
    CXX_COMPILER=g++
elif command -v clang++ >/dev/null 2>&1; then
    CXX_COMPILER=clang++
else
    echo "Erro: No C++ compiler found (g++ ou clang++ required)."
    exit 1
fi

if command -v ninja >/dev/null 2>&1; then
    GENERATOR="Ninja"
else
    GENERATOR="Unix Makefiles"
fi

echo "Using compiler: $CXX_COMPILER"
echo "Using generator: $GENERATOR"

cmake -B "$BUILD_DIR" -S . -G "$GENERATOR" -DCMAKE_CXX_COMPILER=$CXX_COMPILER -DCMAKE_BUILD_TYPE=Release

cmake --build "$BUILD_DIR" --config Release