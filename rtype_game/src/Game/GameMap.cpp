/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameMap
*/

#include "Game/GameMap.hpp"

namespace rtype {

GameMap::GameMap(RealEngine::Registry& registry) : _registry(registry), _levelRunning(false) {}

GameMap::~GameMap() { std::cout << "GameMap destroyed" << std::endl; }

void GameMap::updateLevel(float deltaTime) {
    if (!_levelRunning) {
        return;
    }
    x_level_position += _scrollingSpeed * deltaTime;
    for (auto& block : _blockEntities) {
        auto* position = _registry.get_component<RealEngine::Position>(block);
        if (position) {
            position->x -= _scrollingSpeed * deltaTime;
        }
    }
    removeDeadBlocks();
}

void GameMap::startLevel() {
    _levelRunning = true;
    RealEngine::AssetManager::getInstance().getMusic(_music_name)->play();
}

void GameMap::stopLevel() {
    _levelRunning = false;
    RealEngine::AssetManager::getInstance().getMusic(_music_name)->stop();
}

void GameMap::unloadLevel() {
    _levelRunning = false;
    RealEngine::AssetManager::getInstance().getMusic(_music_name)->stop();
    for (auto block : _blockEntities) {
        if (block) _registry.add_component(block, RealEngine::AutoDestructible{0.0f});
    }
    _blockEntities.clear();
    _backgroundEntities.clear();
    _scrollingSpeed  = 0.0f;
    x_level_position = 0.0f;
    _isMapLoaded     = false;
}

void GameMap::removeDeadBlocks() {
    _blockEntities.erase(std::remove_if(_blockEntities.begin(), _blockEntities.end(),
                                        [](const std::shared_ptr<RealEngine::Entity>& block) {
                                            return block == nullptr;
                                        }),
                         _blockEntities.end());
}

void GameMap::addBackground(std::shared_ptr<RealEngine::Entity> background,
                            RealEngine::ParallaxSystem&         parallaxSystem) {
    _backgroundEntities.push_back(background);
    parallaxSystem.update(_registry, 0.0f);  // this call is to set the background and the scale
}

}  // namespace rtype