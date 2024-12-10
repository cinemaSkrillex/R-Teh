cmake_minimum_required(VERSION 3.10)
project(rtype_game)

# Add sources files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Create the binary
add_executable(r_type ${SOURCES})

# Link lib to binary
target_link_libraries(r_type rtype_engine)
target_link_libraries(r_type rtype_client)

target_include_directories(r_type PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Client
    ${CMAKE_CURRENT_SOURCE_DIR}/include/shared
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

# Copy the include headers from game_engine to rtype_game
add_custom_target(copy_headers_engine_for_game ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/game_engine/include
    ${CMAKE_SOURCE_DIR}/rtype_game/include
)

# Ensure the headers are copied before building rtype_game
add_dependencies(r_type copy_headers_engine_for_game)


if (WIN32)
# Copy all DLLs from the bin directory to the output directory
    add_custom_command(TARGET r_type POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/debug/bin,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/bin>"
            $<TARGET_FILE_DIR:r_type>
    )
endif()