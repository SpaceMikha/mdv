#!/bin/bash

# Mission Design Tool - Quick Build Script
# Run this from the project root directory

echo "MDV- Build Script"
echo "==================================="

# Clean old build
if [ -d "build" ]; then
    echo "Removing old build directory..."
    rm -rf build
fi

# Create fresh build directory
echo "Creating build directory..."
mkdir build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake -G "MinGW Makefiles" ..

if [ $? -ne 0 ]; then
    echo "ERROR: CMake configuration failed!"
    exit 1
fi

# Build
echo "Building project..."
mingw32-make

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed!"
    exit 1
fi

echo ""
echo "==================================="
echo "Build successful!"
echo "==================================="
echo "Run the program with: ./build/mdv.exe"
echo "Or: cd build && ./mdv.exe"
echo ""