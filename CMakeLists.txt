cmake_minimum_required(VERSION 3.10)

# Project name
project(rtype_project)

# Set C++ standard
if (UNIX)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
    # You can add all of the standard flags here
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic -pedantic-errors -Wconversion -Wshadow -Wnon-virtual-dtor -Wold-style-cast")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
endif ()

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif ()

if (MSVC)
    add_compile_options(/std:c++20)
    add_compile_options(/permissive-)
endif ()

# Add debug flags for Valgrind and GDB
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -Og -fno-omit-frame-pointer -fsanitize=address,undefined -fstack-protector-strong")
        set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -fsanitize=address,undefined")
    endif ()
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif ()
endif ()

# Include directories
# Not needed anymore since VCPKG toolchain file is used (CmakePresets.json)


# Add subdirectories
add_subdirectory(Network)

# add_subdirectory(client)
add_subdirectory(GameEngine)
add_subdirectory(RtypeGame)
add_subdirectory(RtypeServer)
add_subdirectory(PongGame)
add_subdirectory(PongServer)
add_subdirectory(RtypeLauncherServer)
add_subdirectory(RtypeLauncherGame)
