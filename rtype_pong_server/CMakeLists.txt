cmake_minimum_required(VERSION 3.10)
project(rtype_pong_server)

# Add sources files
file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

find_package(SFML 2.5 REQUIRED system window graphics network audio)

# Create the binary
add_executable(r_type_pong_server ${SOURCES})

target_link_libraries(r_type_pong_server
        rtype_engine
)

target_include_directories(r_type_pong_server PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Server
        ${CMAKE_CURRENT_SOURCE_DIR}/include/shared
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RtypeServer
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RtypeServer/Callbacks
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RtypeServer/Events
        ${CMAKE_CURRENT_SOURCE_DIR}/Config
        ${CMAKE_BINARY_DIR}/rtype_pong_server/Config
)

# Link lib to binary (r_type_pong_server is linked to rtype_pong_server)
# name: r_type_pong_server (binary) -> rtype_pong_server (lib)
target_link_libraries(r_type_pong_server rtype_server)

# Copy the include headers from game_engine to rtype_game
add_custom_target(copy_headers_engine_for_pong_server ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/game_engine/include
        ${CMAKE_SOURCE_DIR}/rtype_pong_server/include
)

add_custom_target(copy_headers_server_lagcompensation_pong_server ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/lib_utils/LagCompensation/include
        ${CMAKE_SOURCE_DIR}/rtype_pong_server/include
)

add_custom_target(copy_config_pong_server ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/rtype_pong_server/Config
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/rtype_pong_server/Config/server.config
        ${CMAKE_BINARY_DIR}/rtype_pong_server/Config/server.config
)

# add_compile_definitions(CONFIG_FILE_PATH=\"$<TARGET_FILE_DIR:r_type_pong_server>/Config/server.config\")
set(CONFIG_FILE_PATH "${CMAKE_BINARY_DIR}/rtype_pong_server/Config/server.config")

configure_file(${CMAKE_SOURCE_DIR}/rtype_pong_server/Config/configpath.h.in ${CMAKE_BINARY_DIR}/rtype_pong_server/Config/configpath.h)


# Ensure the headers are copied before building rtype_game
add_dependencies(r_type_pong_server copy_headers_engine_for_pong_server)
add_dependencies(r_type_pong_server copy_headers_server_lagcompensation_pong_server)
add_dependencies(r_type_pong_server copy_config_pong_server)
# add_dependencies(rtype_pong_server copy_assets_server)


if (WIN32)
    # Copy all DLLs from the bin directory to the output directory
    add_custom_command(TARGET r_type_pong_server POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/debug/bin,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/bin>"
            $<TARGET_FILE_DIR:r_type_pong_server>
    )
endif ()

# Copy the include headers (client, server, and shared) into the include folder of ../rtype_game and ../rtype_pong_server and ../game_engine
# add_custom_target(copy_headers ALL
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_CURRENT_SOURCE_DIR}/include/Client
#     ${CMAKE_SOURCE_DIR}/rtype_pong_server/include/Client
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_CURRENT_SOURCE_DIR}/include/Server

add_custom_target(copy_headers_protocol_pong_server ALL
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/include/shared/RtypeServerProtocol.hpp
        ${CMAKE_SOURCE_DIR}/rtype_pong/include/shared/RtypeServerProtocol.hpp
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/include/shared/RtypeServerProtocol.tpp
        ${CMAKE_SOURCE_DIR}/rtype_pong/include/shared/RtypeServerProtocol.tpp
)