# Network project
project(Network)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add debug flags for Valgrind
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")


# Include directories
if (UNIX)
    include_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux/include)
endif()
if (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include)
endif()

# Server Source files
file(GLOB_RECURSE NETWORK_SERVER_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Server/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Server/*.hpp"
)

# Client Source files
file(GLOB_RECURSE NETWORK_CLIENT_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Client/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Client/*.hpp"
)

# Build the shared library (DLL on Windows, .so on Linux)
if(WIN32)
    add_library(rtype_server SHARED ${NETWORK_SERVER_SOURCES})
    set_target_properties(rtype_server PROPERTIES
        OUTPUT_NAME "librtype_server"
        SUFFIX ".dll"
    )
    add_library(rtype_client SHARED ${NETWORK_CLIENT_SOURCES})
    set_target_properties(rtype_client PROPERTIES
        OUTPUT_NAME "librtype_client"
        SUFFIX ".dll"
    )
else()
    add_library(rtype_server SHARED ${NETWORK_SERVER_SOURCES})
    set_target_properties(rtype_server PROPERTIES
        OUTPUT_NAME "rtype_server"
        SUFFIX ".so"
    )
    add_library(rtype_client SHARED ${NETWORK_CLIENT_SOURCES})
    set_target_properties(rtype_client PROPERTIES
        OUTPUT_NAME "rtype_client"
        SUFFIX ".so"
    )
endif()


# Define SERVER_LIBRARY_EXPORTS for symbol export
target_compile_definitions(rtype_server PRIVATE SERVER_LIBRARY_EXPORTS CLIENT_LIBRARY_EXPORTS)

# Include directories
target_include_directories(rtype_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set rpath to include the directory containing the shared libraries
# Link pthread for Linux
if(UNIX)
    target_link_libraries(rtype_server pthread)
endif()

# Link ws2_32 for Windows
if(WIN32)
    target_link_libraries(rtype_server ws2_32)
endif()

# Use main and the shared library to build the executable of testing : server and client
# Build the executable and link with the shared library
add_executable(testing_rtype_server TestingServerMain.cpp)
if (UNIX)
    target_link_libraries(testing_rtype_server rtype_server dl)  # Link with rtype_server and dl for dynamic loading on Linux   
endif()
if (WIN32)
    target_link_libraries(testing_rtype_server rtype_server)  # Link with rtype_server on Windows
endif()

add_executable(testing_rtype_client TestingClientMain.cpp)
if (UNIX)
    target_link_libraries(testing_rtype_client rtype_client dl)  # Link with rtype_client and dl for dynamic loading on Linux
endif()
if (WIN32)
    target_link_libraries(testing_rtype_client rtype_client)  # Link with rtype_client on Windows
endif()