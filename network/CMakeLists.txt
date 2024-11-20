# Network project
project(Network)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux/include)

# Source files
file(GLOB_RECURSE NETWORK_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# if you want a  library target
# add_library(rtype_server ${NETWORK_SOURCES})
add_executable(rtype_server ${NETWORK_SOURCES})

# Include directories
target_include_directories(rtype_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link pthread for Linux
if(UNIX)
    target_link_libraries(rtype_server pthread)
endif()

# Link ws2_32 for Windows
if(WIN32)
    target_link_libraries(rtype_server ws2_32)
endif()