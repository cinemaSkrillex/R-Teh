cmake_minimum_required(VERSION 3.10)

# Project name
project(GameEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories
add_subdirectory(network)
# add_subdirectory(client)
add_subdirectory(game_engine)