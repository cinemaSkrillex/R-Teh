cmake_minimum_required(VERSION 3.10)
project(RtypeServer)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RtypeServer
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RtypeServer/Callbacks
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RtypeServer/Events
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RtypeServer/Scenes
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Game/Level
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Game
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Game/Mobs
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Game/Player
        ${CMAKE_CURRENT_SOURCE_DIR}/Config
        ${CMAKE_BINARY_DIR}/RtypeServer/Config
)


# Link lib to binary (r_type_server is linked to RtypeServer)
# name: r_type_server (binary) -> RtypeServer (lib)
target_link_libraries(r_type_server RtypeServer)

# Copy the include headers from GameEngine to RtypeGame
add_custom_target(copy_headers_engine_for_server ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/GameEngine/include
        ${CMAKE_SOURCE_DIR}/RtypeServer/include
)

add_custom_target(copy_headers_server_lagcompensation ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/LibUtils/LagCompensation/include
        ${CMAKE_SOURCE_DIR}/RtypeServer/include
)

add_custom_target(copy_config ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/RtypeServer/Config
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/RtypeServer/Config/server.config
        ${CMAKE_BINARY_DIR}/RtypeServer/Config/server.config
)

# add_compile_definitions(CONFIG_FILE_PATH=\"$<TARGET_FILE_DIR:r_type_server>/Config/server.config\")
set(CONFIG_FILE_PATH "${CMAKE_BINARY_DIR}/RtypeServer/Config/server.config")

configure_file(${CMAKE_SOURCE_DIR}/RtypeServer/Config/configpath.h.in ${CMAKE_BINARY_DIR}/RtypeServer/Config/configpath.h)


# Ensure the headers are copied before building RtypeGame
add_dependencies(r_type copy_headers_engine_for_server)
add_dependencies(r_type copy_headers_server_lagcompensation)
add_dependencies(r_type copy_config)
# add_dependencies(r_type copy_assets_server)


if (WIN32)
    # Copy all DLLs from the bin directory to the output directory
    add_custom_command(TARGET r_type_server POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/debug/bin,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/bin>"
            $<TARGET_FILE_DIR:r_type_server>
    )
endif ()

# Copy the include headers (client, server, and shared) into the include folder of ../RtypeGame and ../RtypeServer and ../GameEngine
# add_custom_target(copy_headers ALL
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_CURRENT_SOURCE_DIR}/include/Client
#     ${CMAKE_SOURCE_DIR}/RtypeServer/include/Client
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_CURRENT_SOURCE_DIR}/include/Server

add_custom_target(copy_headers_protocol ALL
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/include/shared/RtypeServerProtocol.hpp
        ${CMAKE_SOURCE_DIR}/RtypeGame/include/shared/RtypeServerProtocol.hpp
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/include/shared/RtypeServerProtocol.tpp
        ${CMAKE_SOURCE_DIR}/RtypeGame/include/shared/RtypeServerProtocol.tpp
)