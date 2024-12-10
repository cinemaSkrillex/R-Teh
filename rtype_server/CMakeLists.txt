cmake_minimum_required(VERSION 3.10)
project(rtype_server)

# Add sources files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

find_package(SFML 2.5 REQUIRED system window graphics network audio)

# Create the binary
add_executable(r_type_server ${SOURCES})

target_link_libraries(r_type_server
    rtype_engine
)

target_include_directories(r_type_server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Server
    ${CMAKE_CURRENT_SOURCE_DIR}/include/shared
)

# Link lib to binary (r_type_server is linked to rtype_server)
# name: r_type_server (binary) -> rtype_server (lib)
target_link_libraries(r_type_server rtype_server)

# # Copy the include headers from game_engine to rtype_server
# add_custom_target(copy_headers_engine_for_server ALL
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/game_engine/include
#     ${CMAKE_SOURCE_DIR}/rtype_server/include
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/game_engine/include/Core
#     ${CMAKE_SOURCE_DIR}/rtype_server/include/Core
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/game_engine/include/ECS
#     ${CMAKE_SOURCE_DIR}/rtype_server/include/ECS
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/game_engine/include/Media
#     ${CMAKE_SOURCE_DIR}/rtype_server/include/Media
# )

# Ensure the headers are copied before building rtype_server
# add_dependencies(r_type_server copy_headers_engine_for_server)


# Copy the include headers from game_engine to rtype_game
add_custom_target(copy_headers_engine_for_server ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/game_engine/include
    ${CMAKE_SOURCE_DIR}/rtype_server/include
)

# Ensure the headers are copied before building rtype_game
add_dependencies(r_type copy_headers_engine_for_server)


if (WIN32)
    # Copy all DLLs from the bin directory to the output directory
    add_custom_command(TARGET r_type_server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/debug/bin,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/bin>"
            $<TARGET_FILE_DIR:r_type_server>
    )
endif()