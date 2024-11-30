cmake_minimum_required(VERSION 3.10)
project(rtype_game)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#TODO BUILD_SHARED_LIBS(OFF)
#TODO BUILD_SHARED_LIBS(OFF)
#TODO BUILD_SHARED_LIBS(OFF)

# define _WIN32_WINNT for windows
if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

#includes
if (UNIX)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/include)
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux")
    set(SFML_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/share/sfml)
elseif (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/include)
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows")
    set(SFML_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/share/sfml)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Client)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/shared)


# Add sources files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Create the binary
add_executable(r_type ${SOURCES})

# Link pthread for Linux
if(UNIX)
    target_link_libraries(r_type
        pthread
    )
endif()

# Link ws2_32 for Windows
if(WIN32)
    target_link_libraries(r_type
        ws2_32
    )
endif()

# Link lib to binary
target_link_libraries(r_type rtype_engine)
target_link_libraries(r_type rtype_client)

if (WIN32)
# Copy all DLLs from the bin directory to the output directory
    add_custom_command(TARGET r_type POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/debug/bin,${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/bin>"
            $<TARGET_FILE_DIR:r_type>
    )
endif()