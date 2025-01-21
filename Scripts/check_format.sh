#!/bin/bash
set -e

# Run clang-format check
if ! find GameEngine/include GameEngine/src RtypeServer/include RtypeServer/src RtypeGame/include RtypeGame/src Network/include Network/src PongGame/src PongGame/include -type f \( -name "*.hpp" -o -name "*.cpp" \) | xargs clang-format --dry-run --Werror; then
    echo "Code formatting issues detected. Please run './Scripts/format_all.sh' to fix them."
    exit 1
fi