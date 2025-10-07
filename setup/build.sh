#!/bin/bash

# MDV - Mission Design Visualizer - Build Script
# Run this from the project root directory in Git Bash

echo "=================================="
echo "MDV - Build Script"
echo "=================================="
echo ""

# Check if MinGW tools are in PATH
if ! command -v g++ &> /dev/null; then
    echo "ERROR: MinGW g++ not found in PATH!"
    echo "Please run setup.sh first in MSYS2 MinGW64 terminal"
    echo "Then restart Git Bash or run: source ~/.bashrc"
    exit 1
fi

# Check if we're in the project root
if [ ! -f "CMakeLists.txt" ]; then
    echo "ERROR: CMakeLists.txt not found!"
    echo "Please run this script from the MDV project root directory"
    exit 1
fi

# Clean old build
if [ -d "build" ]; then
    echo "Removing old build directory..."
    rm -rf build
fi

# Create fresh build directory
echo "Creating build directory..."
mkdir build
cd build || exit 1

# Configure with CMake
echo ""
echo "Configuring with CMake..."
cmake -G "MinGW Makefiles" ..

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: CMake configuration failed!"
    echo "Make sure all dependencies are installed (run setup.sh)"
    exit 1
fi

# Build
echo ""
echo "Building project..."
mingw32-make

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: Build failed!"
    echo "Check the error messages above"
    exit 1
fi

echo ""
echo "=================================="
echo "Build Successful!"
echo "=================================="
echo ""
echo "Run the program with:"
echo "  ./build/mdv.exe"
echo ""
echo "Or:"
echo "  cd build && ./mdv.exe"
echo ""