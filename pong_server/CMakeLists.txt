cmake_minimum_required(VERSION 3.10)
project(pong_server)

# Add sources files
file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

find_package(SFML 2.5 REQUIRED system window graphics network audio)

# Create the binary
add_executable(pong_server_exe ${SOURCES})

target_link_libraries(pong_server_exe
        rtype_engine
)

target_include_directories(pong_server_exe PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        # ${CMAKE_CURRENT_SOURCE_DIR}/include/Server
        # ${CMAKE_CURRENT_SOURCE_DIR}/include/shared
        # ${CMAKE_CURRENT_SOURCE_DIR}/include/PongServer
        # ${CMAKE_CURRENT_SOURCE_DIR}/include/PongServer/Callbacks
        # ${CMAKE_CURRENT_SOURCE_DIR}/include/PongServer/Events
        # ${CMAKE_CURRENT_SOURCE_DIR}/Config
        # ${CMAKE_BINARY_DIR}/pong_server/Config
)

# Link lib to binary (pong_server_exe is linked to pong_server)
# name: pong_server_exe (binary) -> pong_server (lib)
# target_link_libraries(pong_server_exe pong_server)
target_link_libraries(pong_server_exe rtype_server)

# Copy the include headers from game_engine to rtype_game
add_custom_target(copy_headers_engine_for_server_pong ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/game_engine/include
        ${CMAKE_SOURCE_DIR}/pong_server/include
)

add_custom_target(copy_headers_server_lagcompensation_pong ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/lib_utils/LagCompensation/include
        ${CMAKE_SOURCE_DIR}/pong_server/include
)

# add_custom_target(copy_config_pong ALL
#         COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/pong_server/Config
#         COMMAND ${CMAKE_COMMAND} -E copy
#         ${CMAKE_SOURCE_DIR}/pong_server/Config/server.config
#         ${CMAKE_BINARY_DIR}/pong_server/Config/server.config
# )

# add_compile_definitions(CONFIG_FILE_PATH=\"$<TARGET_FILE_DIR:pong_server_exe>/Config/server.config\")
# set(CONFIG_FILE_PATH "${CMAKE_BINARY_DIR}/pong_server/Config/server.config")

# configure_file(${CMAKE_SOURCE_DIR}/pong_server/Config/configpath.h.in ${CMAKE_BINARY_DIR}/pong_server/Config/configpath.h)


# Ensure the headers are copied before building rtype_game
add_dependencies(pong copy_headers_engine_for_server_pong)
add_dependencies(pong copy_headers_server_lagcompensation_pong)
# add_dependencies(pong copy_config_pong)
# add_dependencies(r_type copy_assets_server)


if (WIN32)
    # Copy all DLLs from the bin directory to the output directory
    add_custom_command(TARGET pong_server_exe POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/debug/bin,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/bin>"
            $<TARGET_FILE_DIR:pong_server_exe>
    )
endif ()

# Copy the include headers (client, server, and shared) into the include folder of ../rtype_game and ../pong_server and ../game_engine
# add_custom_target(copy_headers ALL
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_CURRENT_SOURCE_DIR}/include/Client
#     ${CMAKE_SOURCE_DIR}/pong_server/include/Client
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_CURRENT_SOURCE_DIR}/include/Server

# add_custom_target(copy_headers_protocol ALL
#         COMMAND ${CMAKE_COMMAND} -E copy
#         ${CMAKE_CURRENT_SOURCE_DIR}/include/shared/RtypeServerProtocol.hpp
#         ${CMAKE_SOURCE_DIR}/pong_game/include/shared/RtypeServerProtocol.hpp
#         COMMAND ${CMAKE_COMMAND} -E copy
#         ${CMAKE_CURRENT_SOURCE_DIR}/include/shared/RtypeServerProtocol.tpp
#         ${CMAKE_SOURCE_DIR}/pong_game/include/shared/RtypeServerProtocol.tpp
# )