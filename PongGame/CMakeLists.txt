cmake_minimum_required(VERSION 3.10)
project(PongGame)

# Add sources files
file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Create the binary
add_executable(pong ${SOURCES})

# Link lib to binary
target_link_libraries(pong rtype_engine)
target_link_libraries(pong rtype_client)

target_include_directories(pong PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Client
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Shared
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Game
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

# Copy the include headers from GameEngine to PongGame
add_custom_target(copy_headers_engine_for_game_pong ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/GameEngine/include
        ${CMAKE_SOURCE_DIR}/PongGame/include
)

# Ensure the headers are copied before building PongGame
add_dependencies(pong copy_headers_engine_for_game_pong)

if (WIN32)
    # Copy all DLLs from the bin directory to the output directory
    add_custom_command(TARGET pong POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/debug/bin,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/bin>"
            $<TARGET_FILE_DIR:pong>
    )
endif ()