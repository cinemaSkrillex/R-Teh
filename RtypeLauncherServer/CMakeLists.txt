cmake_minimum_required(VERSION 3.10)
project(RtypeLauncherServer)

# Add sources files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

find_package(SFML 2.5 REQUIRED system window graphics network audio)

# Create the binary
add_executable(rtype_l_auncher_server ${SOURCES})

target_link_libraries(rtype_l_auncher_server
    rtype_engine
)

target_include_directories(rtype_l_auncher_server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Server
    ${CMAKE_CURRENT_SOURCE_DIR}/include/shared
    ${CMAKE_CURRENT_SOURCE_DIR}/include/RtypeServer
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameMap
    ${CMAKE_CURRENT_SOURCE_DIR}/Config
    ${CMAKE_BINARY_DIR}/RtypeLauncherServer/Config
)

# Copy the include headers from GameEngine to RtypeGame
add_custom_target(copy_headers_engine_for_server_launcher ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/GameEngine/include
    ${CMAKE_SOURCE_DIR}/RtypeLauncherServer/include
)

# Ensure the headers are copied before building RtypeGame
add_dependencies(r_type copy_headers_engine_for_server_launcher)
# add_dependencies(r_type copy_assets_server)


if (WIN32)
    # Copy all DLLs from the bin directory to the output directory
    add_custom_command(TARGET rtype_l_auncher_server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/debug/bin,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/bin>"
            $<TARGET_FILE_DIR:rtype_l_auncher_server>
    )
endif()