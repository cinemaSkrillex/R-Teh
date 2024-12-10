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

if (WIN32)
    # Copy all DLLs from the bin directory to the output directory
    add_custom_command(TARGET r_type_server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/debug/bin,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/bin>"
            $<TARGET_FILE_DIR:r_type_server>
    )
endif()