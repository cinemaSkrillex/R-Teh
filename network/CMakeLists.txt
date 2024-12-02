# Network project
project(Network)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add debug flags for Valgrind
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif()
endif()

#define _WIN32_WINNT for windows
if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

# Include directories
if (UNIX)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/include)
endif()
if (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/include)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Client)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/shared)

# Server Source files
file(GLOB_RECURSE NETWORK_SERVER_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Server/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/*.cpp"
)

# Client Source files
file(GLOB_RECURSE NETWORK_CLIENT_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Client/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/*.cpp"
)

# Build the static library (DLL on Windows, .so on Linux)
add_library(rtype_server STATIC ${NETWORK_SERVER_SOURCES})
add_library(rtype_client STATIC ${NETWORK_CLIENT_SOURCES})

# Include directories
target_include_directories(rtype_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set rpath to include the directory containing the shared libraries
# Link pthread for Linux
if(UNIX)
    target_link_libraries(rtype_server pthread)
    add_executable(testing_rtype_server TestingServerMain.cpp)
    target_link_libraries(testing_rtype_server rtype_server)  # Link with rtype_server and dl for dynamic loading on Linux   
    add_executable(testing_rtype_client TestingClientMain.cpp)
    target_link_libraries(testing_rtype_client rtype_client)  # Link with rtype_client and dl for dynamic loading on Linux
endif()

# Link ws2_32 for Windows
if(WIN32)
    target_link_libraries(rtype_server ws2_32)
    add_executable(testing_rtype_server TestingServerMain.cpp)
    target_link_libraries(testing_rtype_server rtype_server)  # Link with rtype_server on Windows
    add_executable(testing_rtype_client TestingClientMain.cpp)
    target_link_libraries(testing_rtype_client rtype_client)  # Link with rtype_client on Windows
endif()