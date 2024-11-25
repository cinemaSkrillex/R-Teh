cmake_minimum_required(VERSION 3.10)
project(rtype_game)

# Ajouter les chemins d'inclusion
include_directories(${CMAKE_SOURCE_DIR}/../game_engine/include)

# Ajouter les fichiers sources
set(SOURCES main.cpp)

# Créer l'exécutable
add_executable(r_type ${SOURCES})

# Lier l'exécutable avec la bibliothèque statique
target_link_libraries(r_type rtype_engine)