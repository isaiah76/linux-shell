#!/bin/bash

set -e

BUILD_DIR="build"

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

echo "Running CMake..."
cmake ..

echo "Building project..."
make

# Ask the user if they want to install system-wide
read -p "Do you want to install linux-shell system-wide? [y/N] " install_choice
if [[ "$install_choice" =~ ^[Yy]$ ]]; then
    echo "Installing linux-shell..."
    sudo make install
fi

echo "Build complete. To run the shell, type: linux-shell"

