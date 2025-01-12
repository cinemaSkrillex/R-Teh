/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameMap
*/

#include "Game/GameMap.hpp"

GameMap::GameMap(RealEngine::Registry& registry) : _registry(registry), _levelStarted(false) {}

GameMap::~GameMap() { std::cout << "GameMap destroyed" << std::endl; }

void GameMap::updateLevel(float deltaTime) {
    if (!_levelStarted) {
        return;
    }
    x_level_position += _scrollingSpeed * deltaTime;
    for (auto& block : _blockEntities) {
        auto* position = _registry.get_component<RealEngine::Position>(block);
        if (position) {
            position->x -= _scrollingSpeed * deltaTime;
        }
    }
}

void GameMap::removeDeadBlocks() {
    _blockEntities.erase(std::remove_if(_blockEntities.begin(), _blockEntities.end(),
                                        [](const std::shared_ptr<RealEngine::Entity>& block) {
                                            return block == nullptr;
                                        }),
                         _blockEntities.end());
}
