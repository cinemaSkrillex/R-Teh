#!/bin/bash
find game_engine/include game_engine/src rtype_server/include rtype_server/src rtype_game/include rtype_game/src network/include network/src rtype_launcher_game/include rtype_launcher_game/src rtype_launcher_server/include rtype_launcher_server/src -type f \( -name "*.hpp" -o -name "*.cpp" \) | xargs clang-format -i
