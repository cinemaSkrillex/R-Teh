cmake_minimum_required(VERSION 3.10)
project(rtype_server)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
if (UNIX)
    include_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux/include)
endif()
if (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include)
endif()

#add includes files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add sources files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

# Create the binary
add_executable(r_type_server ${SOURCES})

# Link pthread for Linux
if(UNIX)
    target_link_libraries(r_type_server
        pthread
        sfml-system
    )
endif()

# Link ws2_32 for Windows
if(WIN32)
    target_link_libraries(r_type_server
        ws2_32
        sfml-system
    )
endif()

# Link lib to binary
target_link_libraries(r_type_server rtype_server)