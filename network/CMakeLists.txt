# Network project
project(Network)

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

# Source files
file(GLOB_RECURSE NETWORK_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)


# Build the shared library (DLL on Windows, .so on Linux)
if(WIN32)
    add_library(rtype_server SHARED ${NETWORK_SOURCES})
    set_target_properties(rtype_server PROPERTIES
        OUTPUT_NAME "rtype_server"
        SUFFIX ".dll"
    )
else()
    add_library(rtype_server SHARED ${NETWORK_SOURCES})
    set_target_properties(rtype_server PROPERTIES
        OUTPUT_NAME "rtype_server"
        SUFFIX ".so"
    )
endif()


# Define SERVER_LIBRARY_EXPORTS for symbol export
target_compile_definitions(rtype_server PRIVATE SERVER_LIBRARY_EXPORTS)

# Include directories
target_include_directories(rtype_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set rpath to include the directory containing the shared libraries
# Link pthread for Linux
if(UNIX)
    target_link_libraries(rtype_server pthread)
endif()

# Link ws2_32 for Windowsd
if(WIN32)
    target_link_libraries(rtype_server ws2_32)
endif()

# Build the executable and link with the shared library
add_executable(rtype_client main.cpp)
if (UNIX)
    target_link_libraries(rtype_client rtype_server dl)  # Link with rtype_server and dl for dynamic loading on Linux   
endif()
if (WIN32)
    target_link_libraries(rtype_client rtype_server)  # Link with rtype_server on Windows
endif()
