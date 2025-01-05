#!/bin/bash
find game_engine/include game_engine/src rtype_server/include rtype_server/src rtype_game/include rtype_game/src network/include network/src -type f \( -name "*.hpp" -o -name "*.cpp" \) | xargs clang-format --dry-run --Werror
