/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MapUpdater
*/

#include "RtypeServer/RtypeServer.hpp"

MapUpdater::MapUpdater(RealEngine::Registry& registry, std::shared_ptr<GameMap> map)
    : _registry(registry), _map(map) {}

void MapUpdater::update(float deltaTime) {
    if (!_map || !_map->isLoaded()) {
        std::cerr << "Error: Server map is not loaded" << std::endl;
        return;
    }

    updateBlocks(deltaTime);
    updateMapPosition(deltaTime);
}

void MapUpdater::updateBlocks(float deltaTime) {
    auto& blocks = _map->getBlockEntities();

    for (auto it = blocks.begin(); it != blocks.end();) {
        auto blockEntity = (*it)->getEntity();
        if (!blockEntity) {
            std::cerr << "Error: Block entity is null" << std::endl;
            it = blocks.erase(it);
            continue;
        }

        auto* position = _registry.get_component<RealEngine::Position>(*blockEntity);
        if (!position) {
            std::cerr << "Error: Block position is null" << std::endl;
            it = blocks.erase(it);
            continue;
        }
        if (position->x < 0) {
            _registry.remove_entity(*blockEntity);
            it = blocks.erase(it);
            continue;
        }

        position->x -= _map->getScrollingSpeed() * deltaTime;
        ++it;
    }
}

void MapUpdater::updateMapPosition(float deltaTime) {
    float map_x_level_position = _map->getXLevelPosition();
    map_x_level_position += _map->getScrollingSpeed() * deltaTime;
    _map->setXLevelPosition(map_x_level_position);
}