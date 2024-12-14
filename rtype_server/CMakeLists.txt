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
target_link_libraries(r_type_server utils)

# Copy the include headers from game_engine to rtype_game
add_custom_target(copy_headers_engine_for_server ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/game_engine/include
    ${CMAKE_SOURCE_DIR}/rtype_server/include
)

add_custom_target(copy_headers_server_parser ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/lib_utils/PeterParser/include
    ${CMAKE_SOURCE_DIR}/rtype_server/include
)
add_custom_target(copy_headers_server_lagcompensation ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/lib_utils/LagCompensation/include
    ${CMAKE_SOURCE_DIR}/rtype_server/include
)

# Copy the configuration file to the build directory
# After that, no need to recompile the server to update the configuration, go to build/default/rtype_server/Config/server.config
add_custom_target(copy_config ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/rtype_server/Config
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/rtype_server/Config/server.config
    ${CMAKE_BINARY_DIR}/rtype_server/Config/server.config
    )

add_compile_definitions(CONFIG_FILE_PATH=\"$<TARGET_FILE_DIR:rtype_server>/Config/server.config\")

# Copy the assets to the build directory
add_custom_target(copy_assets_server ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/rtype_server/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/rtype_server/assets
)

add_compile_definitions(ASSETS_PATH=\"$<TARGET_FILE_DIR:r_type_server>/assets\")


# Ensure the headers are copied before building rtype_game
add_dependencies(r_type copy_headers_engine_for_server)
add_dependencies(r_type copy_headers_server_parser)
add_dependencies(r_type copy_headers_server_lagcompensation)
add_dependencies(r_type copy_config)
add_dependencies(r_type copy_assets_server)


if (WIN32)
    # Copy all DLLs from the bin directory to the output directory
    add_custom_command(TARGET r_type_server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/debug/bin,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/bin>"
            $<TARGET_FILE_DIR:r_type_server>
    )
endif()