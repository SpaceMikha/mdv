#!/bin/bash

# MDV - Auto Setup Script
# This script sets up the development environment on a fresh Windows machine with MSYS2

echo "=================================="
echo "MDV - Auto Setup"
echo "=================================="
echo ""

# Check if running in MSYS2
if [[ "$OSTYPE" != "msys" ]]; then
    echo "ERROR: This script must be run in MSYS2 MinGW64 terminal!"
    echo "Please open 'MSYS2 MinGW64' (purple icon) and run this script again."
    exit 1
fi

echo "Step 1: Updating MSYS2 package database..."
pacman -Syu --noconfirm

echo ""
echo "Step 2: Installing MinGW64 toolchain (gcc, g++, make)..."
pacman -S --needed --noconfirm mingw-w64-x86_64-toolchain

echo ""
echo "Step 3: Installing CMake..."
pacman -S --needed --noconfirm mingw-w64-x86_64-cmake

echo ""
echo "Step 4: Installing Raylib..."
pacman -S --needed --noconfirm mingw-w64-x86_64-raylib

echo ""
echo "Step 5: Installing Git (if not already installed)..."
pacman -S --needed --noconfirm git

echo ""
echo "Step 6: Verifying installations..."
echo -n "GCC version: "
g++ --version | head -n 1
echo -n "CMake version: "
cmake --version | head -n 1
echo -n "Git version: "
git --version

echo ""
echo "Step 7: Adding MinGW to PATH for Git Bash..."
BASHRC="$HOME/.bashrc"
PATH_LINE='export PATH="/c/msys64/mingw64/bin:$PATH"'

if ! grep -q "$PATH_LINE" "$BASHRC" 2>/dev/null; then
    echo "$PATH_LINE" >> "$BASHRC"
    echo "Added MinGW to PATH in ~/.bashrc"
else
    echo "PATH already configured in ~/.bashrc"
fi

echo ""
echo "Step 8: Clone/Setup project (optional)..."
read -p "Do you want to clone the project from a git repository? (y/n): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    read -p "Enter git repository URL: " REPO_URL
    read -p "Enter target directory name (default: mdv): " DIR_NAME
    DIR_NAME=${DIR_NAME:-mdv}
    
    if [ -d "$DIR_NAME" ]; then
        echo "Directory $DIR_NAME already exists. Skipping clone."
    else
        git clone "$REPO_URL" "$DIR_NAME"
        cd "$DIR_NAME"
    fi
else
    echo "Skipping project clone. You can manually copy your project files."
fi

echo ""
echo "=================================="
echo "Setup Complete!"
echo "=================================="
echo ""
echo "Installed packages:"
echo "  - MinGW64 toolchain (gcc, g++, make)"
echo "  - CMake"
echo "  - Raylib"
echo "  - Git"
echo ""
echo "Next steps:"
echo "  1. Copy your project files to this computer (or clone from git)"
echo "  2. Navigate to project directory: cd /c/path/to/mdv"
echo "  3. Create build directory: mkdir build && cd build"
echo "  4. Configure project: cmake -G \"MinGW Makefiles\" .."
echo "  5. Build project: mingw32-make"
echo "  6. Run: ./mdv.exe"
echo ""
echo "To use these tools in Git Bash, restart your terminal or run:"
echo "  source ~/.bashrc"
echo ""