cmake_minimum_required(VERSION 3.10)
project(LibUtils)

# create the library

#name will be `LibUtils.a` or `LibUtils.lib`

# target the include directories

target_include_directories(utils PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/LagCompensation/include
)


