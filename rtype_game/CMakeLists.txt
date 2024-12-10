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
)

if (WIN32)
# Copy all DLLs from the bin directory to the output directory
    add_custom_command(TARGET r_type POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/debug/bin,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/bin>"
            $<TARGET_FILE_DIR:r_type>
    )
endif()