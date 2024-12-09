# Project configuration
cmake_minimum_required(VERSION 3.14)
project(game_engine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
set(CMAKE_VERBOSE_MAKEFILE ON)
#TODO BUILD_SHARED_LIBS(OFF)
#TODO BUILD_SHARED_LIBS(OFF)
#TODO BUILD_SHARED_LIBS(OFF)
#TODO BUILD_SHARED_LIBS(OFF)
#TODO BUILD_SHARED_LIBS(OFF)

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
    if (MSVC)
        add_compile_options(/std:c++17)
        add_compile_options(/permissive-)
        add_compile_options(/showIncludes)
    endif()
endif()

# Include directories (platform-specific)
if (UNIX)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/include)
    # set(SFML_STATIC_LIBRARIES TRUE)
    set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux)
    set(SFML_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/share/sfml)
elseif (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/include)
    # set(SFML_STATIC_LIBRARIES TRUE)
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows")
    set(SFML_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/share/sfml)

endif()
    
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Config)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Components)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Entities)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Enum)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Registry)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/SparseArray)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Systems)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Media)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Scripting)


# Find dependencies
find_package(SFML 2.5 REQUIRED system window graphics network audio)
# find_package(Lua REQUIRED)
# find_package(OpenAL REQUIRED)

# Source files
file(GLOB_RECURSE NETWORK_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Library target
add_library(rtype_engine STATIC ${NETWORK_SOURCES})

# Include directories
target_include_directories(rtype_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Config
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ECS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Components
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Entities
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Enum
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Registry
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/SparseArray
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Systems
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Media
)
# Link libraries
if(UNIX)
    target_link_libraries(rtype_engine
        pthread
        sfml-system
        sfml-window
        sfml-graphics
        sfml-network
        sfml-audio
    )
elseif(WIN32)
    target_link_libraries(rtype_engine
        ws2_32
        sfml-system
        sfml-window
        sfml-graphics
        sfml-network
        sfml-audio
    )

endif()
