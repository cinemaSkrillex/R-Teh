#!/bin/bash
set -e

# Run clang-format check
if ! find game_engine/include game_engine/src rtype_server/include rtype_server/src rtype_game/include rtype_game/src network/include network/src -type f \( -name "*.hpp" -o -name "*.cpp" \) | xargs clang-format --dry-run --Werror; then
    echo "Code formatting issues detected. Please run './scripts/format_all.sh' to fix them."
    exit 1
fi