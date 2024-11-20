# Network project
project(Network)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux/include)

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
target_link_libraries(rtype_client rtype_server dl)  # Link with rtype_server and dl for dynamic loading

# Set rpath for the executable to include the directory containing the shared libraries
#set_target_properties(rtype_client PROPERTIES
#    BUILD_RPATH "${CMAKE_CURRENT_BINARY_DIR}/network;${CMAKE_CURRENT_BINARY_DIR}/game_engine"
#    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
#)

# Custom target to set LD_LIBRARY_PATH and run the executable
#add_custom_target(run
#    COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${CMAKE_CURRENT_BINARY_DIR}/network:${CMAKE_CURRENT_BINARY_DIR}/game_engine ./rtype_client 3500
#    DEPENDS rtype_client
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#)