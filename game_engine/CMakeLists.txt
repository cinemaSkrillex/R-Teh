# Project configuration
cmake_minimum_required(VERSION 3.14)
project(game_engine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

# Include directories (platform-specific)
if (UNIX)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/include)
    # set(SFML_STATIC_LIBRARIES TRUE)
    set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux)
    set(SFML_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/share/sfml)
    set(LUA_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/share/lua)
    set(LUA_LIBRARIES "${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/lib/liblua.a")
    set(LUA_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/include")
    # set(OPENAL_DIR ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux/share/openal)
    # set(OPENAL_LIBRARY "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux/lib/libopenal.a")
elseif (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/include)
    # set(SFML_STATIC_LIBRARIES TRUE)
    set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows)
    set(SFML_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/share/sfml)
    set(LUA_DIR ${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/share/lua)
    set(LUA_LIBRARIES "${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/lib/lua53.lib")
    set(LUA_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/include")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Config)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Components)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Entities)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Enum)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Registry)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/SparseArray)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Systems)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Media)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Scripting)

# Find dependencies
find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
find_package(Lua REQUIRED)
# find_package(OpenAL REQUIRED)

# Source files
file(GLOB_RECURSE NETWORK_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Library target
add_library(rtype_engine STATIC ${NETWORK_SOURCES})

# Include directories
target_include_directories(rtype_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# message(STATUS "OPENAL_LIBRARY: ${OPENAL_LIBRARY}")

# Link libraries
if(UNIX)
    target_link_libraries(rtype_engine
        pthread
        sfml-system
        sfml-window
        sfml-graphics
        sfml-network
        sfml-audio
        ${LUA_LIBRARIES}
    )
elseif(WIN32)
    target_link_libraries(rtype_engine
        ws2_32
        sfml-system
        sfml-window
        sfml-graphics
        sfml-network
        sfml-audio
        ${LUA_LIBRARIES}
    )

    # Copy all DLLs from the bin directory to the output directory
    add_custom_command(TARGET rtype_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/bin"
            $<TARGET_FILE_DIR:rtype_engine>
    )
endif()
