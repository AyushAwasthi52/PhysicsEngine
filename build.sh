#!/bin/bash

set -e

echo "Cleaning build directory..."

rm -rf build

mkdir build

cd build

echo "Configuring CMake..."

cmake .. -G Ninja

echo "Building..."

ninja

echo "Running..."

./PhysicsEngine2D