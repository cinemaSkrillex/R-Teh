/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RtypeServerUpdateMap
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

void RtypeServer::updateMapState(float deltaTime) {
    auto       map = _game_instance->getMap();
    MapUpdater mapUpdater(_game_instance->getRegistryRef(), map);
    mapUpdater.update(deltaTime);
}