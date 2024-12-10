cmake_minimum_required(VERSION 3.10)

# Project name
project(rtype_project)

# Set C++ standard
if (UNIX)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
endif()

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

if(MSVC)
    add_compile_options(/std:c++17)
    add_compile_options(/permissive-)
endif()

# Include directories
if (UNIX)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/include)
    set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux)
    set(SFML_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/share/sfml)
    set(OPENAL_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/share/openal)
endif()
if (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/include)
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows")
    set(SFML_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/share/sfml)
    set(OPENAL_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/share/openal)
    set(OPENAL_LIBRARY "${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/lib/OpenAL32.lib")
endif()

# Add subdirectories
add_subdirectory(network)

# add_subdirectory(client)
add_subdirectory(game_engine)
add_subdirectory(rtype_game)
add_subdirectory(rtype_server)