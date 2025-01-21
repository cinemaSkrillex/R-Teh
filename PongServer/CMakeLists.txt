cmake_minimum_required(VERSION 3.10)
project(PongServer)

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
        # ${CMAKE_BINARY_DIR}/PongServer/Config
)

# Link lib to binary (pong_server_exe is linked to PongServer)
# name: pong_server_exe (binary) -> PongServer (lib)
# target_link_libraries(pong_server_exe PongServer)
target_link_libraries(pong_server_exe RtypeServer)

# Copy the include headers from GameEngine to RtypeGame
add_custom_target(copy_headers_engine_for_server_pong ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/GameEngine/include
        ${CMAKE_SOURCE_DIR}/PongServer/include
)

add_custom_target(copy_headers_server_lagcompensation_pong ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/LibUtils/LagCompensation/include
        ${CMAKE_SOURCE_DIR}/PongServer/include
)

# add_custom_target(copy_config_pong ALL
#         COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/PongServer/Config
#         COMMAND ${CMAKE_COMMAND} -E copy
#         ${CMAKE_SOURCE_DIR}/PongServer/Config/server.config
#         ${CMAKE_BINARY_DIR}/PongServer/Config/server.config
# )

# add_compile_definitions(CONFIG_FILE_PATH=\"$<TARGET_FILE_DIR:pong_server_exe>/Config/server.config\")
# set(CONFIG_FILE_PATH "${CMAKE_BINARY_DIR}/PongServer/Config/server.config")

# configure_file(${CMAKE_SOURCE_DIR}/PongServer/Config/configpath.h.in ${CMAKE_BINARY_DIR}/PongServer/Config/configpath.h)


# Ensure the headers are copied before building RtypeGame
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

# Copy the include headers (client, server, and shared) into the include folder of ../RtypeGame and ../PongServer and ../GameEngine
# add_custom_target(copy_headers ALL
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_CURRENT_SOURCE_DIR}/include/Client
#     ${CMAKE_SOURCE_DIR}/PongServer/include/Client
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_CURRENT_SOURCE_DIR}/include/Server

# add_custom_target(copy_headers_protocol ALL
#         COMMAND ${CMAKE_COMMAND} -E copy
#         ${CMAKE_CURRENT_SOURCE_DIR}/include/shared/RtypeServerProtocol.hpp
#         ${CMAKE_SOURCE_DIR}/PongGame/include/shared/RtypeServerProtocol.hpp
#         COMMAND ${CMAKE_COMMAND} -E copy
#         ${CMAKE_CURRENT_SOURCE_DIR}/include/shared/RtypeServerProtocol.tpp
#         ${CMAKE_SOURCE_DIR}/PongGame/include/shared/RtypeServerProtocol.tpp
# )