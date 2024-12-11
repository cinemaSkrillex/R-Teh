cmake_minimum_required(VERSION 3.10)
project(lib_utils)

# Add sources files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/PeterParser/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/LagComposensation/src/*.cpp"
)

# create the library

#name will be `lib_utils.a` or `lib_utils.lib`
add_library(utils STATIC ${SOURCES})

# target the include directories

target_include_directories(utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/include
    ${CMAKE_CURRENT_SOURCE_DIR}/LagComposensation/include
)