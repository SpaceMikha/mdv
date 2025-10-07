#!/bin/bash

# MDV - Mission Design Visualizer - Setup Script
# Run this in MSYS2 MinGW64 terminal on a fresh Windows machine

echo "=================================="
echo "MDV - Auto Setup for New PC"
echo "=================================="
echo ""

# Check if running in MSYS2
if [[ "$OSTYPE" != msys* && "$OSTYPE" != mingw* ]]; then
    echo "ERROR: This script must be run in MSYS2 MinGW64 terminal!"
    echo "Please:"
    echo "  1. Install MSYS2 from https://www.msys2.org/"
    echo "  2. Open 'MSYS2 MinGW64' (purple icon)"
    echo "  3. Run this script again"
    exit 1
fi

echo "Step 1/5: Updating MSYS2 package database..."
pacman -Syu --noconfirm

echo ""
echo "Step 2/5: Installing MinGW64 toolchain (gcc, g++, make)..."
pacman -S --needed --noconfirm mingw-w64-x86_64-toolchain

echo ""
echo "Step 3/5: Installing CMake..."
pacman -S --needed --noconfirm mingw-w64-x86_64-cmake

echo ""
echo "Step 4/5: Installing Raylib graphics library..."
pacman -S --needed --noconfirm mingw-w64-x86_64-raylib

echo ""
echo "Step 5/5: Installing GLFW (required by Raylib)..."
pacman -S --needed --noconfirm mingw-w64-x86_64-glfw

echo ""
echo "Verifying installations..."
echo "=================================="
g++ --version | head -n 1
cmake --version | head -n 1
echo "Raylib: $(pacman -Q mingw-w64-x86_64-raylib)"
echo "GLFW: $(pacman -Q mingw-w64-x86_64-glfw)"
echo "=================================="

echo ""
echo "Configuring Git Bash compatibility..."
BASHRC="$HOME/.bashrc"
PATH_LINE='export PATH="/c/msys64/mingw64/bin:$PATH"'

if ! grep -q "$PATH_LINE" "$BASHRC" 2>/dev/null; then
    echo "$PATH_LINE" >> "$BASHRC"
    echo "Added MinGW to PATH in ~/.bashrc"
else
    echo "PATH already configured in ~/.bashrc"
fi

echo ""
echo "=================================="
echo "Setup Complete!"
echo "=================================="
echo ""
echo "Next steps:"
echo "  1. Close this MSYS2 terminal"
echo "  2. Open Git Bash"
echo "  3. Navigate to your project: cd /c/path/to/mdv"
echo "  4. Run: ./build.sh"
echo ""
echo "Note: The first time you use Git Bash after setup, run:"
echo "  source ~/.bashrc"
echo ""