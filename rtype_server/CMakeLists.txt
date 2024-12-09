cmake_minimum_required(VERSION 3.10)
project(rtype_server)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)

#TODO BUILD_SHARED_LIBS(OFF)
#TODO BUILD_SHARED_LIBS(OFF)
#TODO BUILD_SHARED_LIBS(OFF)

# define _WIN32_WINNT for windows
if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

# Include directories
if (UNIX)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/include)
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux")
    set(SFML_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/share/sfml)
    set(OPENAL_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/share/openal)
elseif (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/include)
    # set(SFML_STATIC_LIBRARIES TRUE)
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows")
    set(SFML_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/share/sfml)
    set(OPENAL_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/share/openal)
    set(OPENAL_LIBRARY "${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/lib/OpenAL32.lib")
endif()

#add includes files

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/shared)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../game_engine/include)
include_directories(${CMAKE_SOURCE_DIR}/../game_engine/include)
include_directories(${CMAKE_SOURCE_DIR}/../game_engine/include/Config)
include_directories(${CMAKE_SOURCE_DIR}/../game_engine/include/Core)
include_directories(${CMAKE_SOURCE_DIR}/../game_engine/include/ECS)
include_directories(${CMAKE_SOURCE_DIR}/../game_engine/include/ECS/Components)
include_directories(${CMAKE_SOURCE_DIR}/../game_engine/include/ECS/Entities)
include_directories(${CMAKE_SOURCE_DIR}/../game_engine/include/ECS/Enum)
include_directories(${CMAKE_SOURCE_DIR}/../game_engine/include/ECS/Registry)
include_directories(${CMAKE_SOURCE_DIR}/../game_engine/include/ECS/SparseArray)
include_directories(${CMAKE_SOURCE_DIR}/../game_engine/include/ECS/Systems)
include_directories(${CMAKE_SOURCE_DIR}/../game_engine/include/Media)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Game)

# Add sources files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

find_package(SFML 2.5 REQUIRED system window graphics network audio)

# Create the binary
add_executable(r_type_server ${SOURCES})

# Link pthread for Linux
if(UNIX)
    target_link_libraries(r_type_server
        pthread
        sfml-system
        sfml-window
        sfml-graphics
        sfml-network
        sfml-audio
        rtype_engine
    )
endif()

# Link ws2_32 for Windows
if(WIN32)
    target_link_libraries(r_type_server
        ws2_32
        sfml-system
        sfml-window
        sfml-graphics
        sfml-network
        sfml-audio
        rtype_engine
    )
endif()

# Link lib to binary
target_link_libraries(r_type_server rtype_server)

if (WIN32)
    # Copy all DLLs from the bin directory to the output directory
    add_custom_command(TARGET r_type_server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/debug/bin,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/bin>"
            $<TARGET_FILE_DIR:r_type_server>
    )
endif()