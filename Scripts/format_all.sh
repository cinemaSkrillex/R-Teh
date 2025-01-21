#!/bin/bash
find GameEngine/include GameEngine/src RtypeServer/include RtypeServer/src RtypeGame/include RtypeGame/src Network/include Network/src RtypeLauncherGame/include RtypeLauncherGame/src RtypeLauncherServer/include RtypeLauncherServer/src -type f \( -name "*.hpp" -o -name "*.cpp" \) | xargs clang-format -i
