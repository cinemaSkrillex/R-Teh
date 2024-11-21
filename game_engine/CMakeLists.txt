# Network project
project(game_engine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
else()
    message(FATAL_ERROR "VCPKG_ROOT environment variable is not set")
endif()

# Include directories
if (UNIX)
    include_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux/include)
endif()
if (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include)
endif()

if(WIN32)
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows")
endif()

if (WIN32)
    set(SFML_DIR ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/share/sfml)
    set(LUA_DIR ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/share/lua)
endif()



# Find Lua
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
find_package(Lua REQUIRED)

# Source files
file(GLOB_RECURSE NETWORK_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# if you want a  library target
add_executable(rtype_engine ${NETWORK_SOURCES})

# Include directories
target_include_directories(rtype_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link pthread for Linux
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
endif()

# Link ws2_32 for Windows
if(WIN32)
    target_link_libraries(rtype_engine
    ws2_32
    sfml-system
    sfml-window
    sfml-graphics
    sfml-network
    sfml-audio
    ${LUA_LIBRARIES}
    )
endif()