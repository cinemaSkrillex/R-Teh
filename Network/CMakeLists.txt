# Network project
project(Network)



# Include directories
if (UNIX)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-linux/include)
endif()
if (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/build/default/vcpkg_installed/x64-windows/include)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Client)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Shared)

# Server Source files
file(GLOB_RECURSE NETWORK_SERVER_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Server/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Shared/*.cpp"
)

# Client Source files
file(GLOB_RECURSE NETWORK_CLIENT_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Client/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Shared/*.cpp"
)

# Build the static library (DLL on Windows, .so on Linux)
add_library(RtypeServer STATIC ${NETWORK_SERVER_SOURCES})
add_library(rtype_client STATIC ${NETWORK_CLIENT_SOURCES})

# Include directories
target_include_directories(RtypeServer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set rpath to include the directory containing the Shared libraries
# Link pthread for Linux
if(UNIX)
    target_link_libraries(RtypeServer pthread)
    add_executable(testing_rtype_server TestingServerMain.cpp)
    target_link_libraries(testing_rtype_server RtypeServer)  # Link with RtypeServer and dl for dynamic loading on Linux   
    add_executable(testing_rtype_client TestingClientMain.cpp)
    target_link_libraries(testing_rtype_client rtype_client)
endif()

# Link ws2_32 for Windows
if(WIN32)
    target_link_libraries(RtypeServer ws2_32)
    add_executable(testing_rtype_server TestingServerMain.cpp)
    target_link_libraries(testing_rtype_server RtypeServer)  # Link with RtypeServer on Windows
    add_executable(testing_rtype_client TestingClientMain.cpp)
    target_link_libraries(testing_rtype_client rtype_client)
endif()

# Copy the include headers (client, server, and Shared) into the include folder of ../RtypeGame and ../RtypeServer and ../GameEngine
add_custom_target(copy_headers ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Client
    ${CMAKE_SOURCE_DIR}/RtypeServer/include/Client
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Server
    ${CMAKE_SOURCE_DIR}/RtypeServer/include/Server
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Shared
    ${CMAKE_SOURCE_DIR}/RtypeServer/include/Shared
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Client
    ${CMAKE_SOURCE_DIR}/RtypeGame/include/Client
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Server
    ${CMAKE_SOURCE_DIR}/RtypeGame/include/Server
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Shared
    ${CMAKE_SOURCE_DIR}/RtypeGame/include/Shared
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Shared
    ${CMAKE_SOURCE_DIR}/GameEngine/include/Shared
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Client
    ${CMAKE_SOURCE_DIR}/GameEngine/include/Client
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Server/TCPServer.hpp
    ${CMAKE_SOURCE_DIR}/RtypeLauncherServer/include/Server/TCPServer.hpp
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Server/TCPServer.hpp
    ${CMAKE_SOURCE_DIR}/RtypeLauncherGame/include/Server/TCPServer.hpp
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Server/TCPServer.cpp
    ${CMAKE_SOURCE_DIR}/RtypeLauncherGame/src/Server/TCPServer.cpp
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Client/TCPClient.cpp
    ${CMAKE_SOURCE_DIR}/RtypeLauncherGame/src/Client/TCPClient.cpp
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DynamicLibrary
    ${CMAKE_SOURCE_DIR}/RtypeLauncherGame/include/DynamicLibrary
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DynamicLibrary
    ${CMAKE_SOURCE_DIR}/RtypeLauncherServer/include/DynamicLibrary
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Server/TCPServer.cpp
    ${CMAKE_SOURCE_DIR}/RtypeLauncherServer/src/Server/TCPServer.cpp
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Client/TCPClient.cpp
    ${CMAKE_SOURCE_DIR}/RtypeLauncherServer/src/Client/TCPClient.cpp
)

# Ensure the headers are copied before building RtypeServer and RtypeGame
add_dependencies(RtypeServer copy_headers)
add_dependencies(rtype_client copy_headers)
