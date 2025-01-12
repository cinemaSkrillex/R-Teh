cmake_minimum_required(VERSION 3.10)
project(lib_utils)

# create the library

#name will be `lib_utils.a` or `lib_utils.lib`

# target the include directories

target_include_directories(utils PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/LagCompensation/include
)


