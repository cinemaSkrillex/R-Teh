# Project configuration
cmake_minimum_required(VERSION 3.14)
project(GameEngine)

if (UNIX)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
endif ()

# Find dependencies
find_package(SFML REQUIRED system window graphics network audio)
find_package(jsoncpp CONFIG REQUIRED)

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
if (UNIX)
    target_link_libraries(rtype_engine
            pthread
            sfml-system
            sfml-window
            sfml-graphics
            sfml-network
            sfml-audio
            JsonCpp::JsonCpp
    )
elseif (WIN32)
    target_link_libraries(rtype_engine
            ws2_32
            sfml-system
            sfml-window
            sfml-graphics
            sfml-network
            sfml-audio
            JsonCpp::JsonCpp
    )
endif ()
