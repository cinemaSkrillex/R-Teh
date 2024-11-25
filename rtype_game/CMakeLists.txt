cmake_minimum_required(VERSION 3.10)
project(rtype_game)

#add includes files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add sources files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)
set(SOURCES main.cpp)

# Create the binary
add_executable(r_type ${SOURCES})

# Link lib to binary
target_link_libraries(r_type rtype_engine)