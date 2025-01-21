/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameMap
*/

#include "Game/GameMap.hpp"

#include <Game.hpp>

namespace rtype {

// GameMap::GameMap(RealEngine::Registry& registry) : _registry(registry), _levelRunning(false) {}

GameMap::GameMap(RealEngine::Registry& registry, Game* game)
    : _registry(registry), _game(game), _levelRunning(false) {
    std::cout << "GameMap created at: " << this << std::endl;
}
GameMap::~GameMap() { std::cout << "GameMap destroyed at: " << this << std::endl; }

void GameMap::updateLevel(float deltaTime) {
    if (!_levelRunning) {
        return;
    }
    _xLevelPosition += _scrollingSpeed * deltaTime;
    removeDeadBlocks();
}

void GameMap::startLevel() {
    _levelRunning = true;
    if (!_musicName.empty()) {
        RealEngine::AssetManager::getInstance().getMusic(_musicName)->play();
    }
}

void GameMap::stopLevel() {
    _levelRunning = false;
    if (!_musicName.empty()) {
        RealEngine::AssetManager::getInstance().getMusic(_musicName)->stop();
    }
}

void GameMap::removeDeadBlocks() {
    auto entities = _game->getEntities();
    for (auto it = entities.begin(); it != entities.end();) {
        auto* position = _registry.getComponent<RealEngine::Position>(it->second.entity);
        if (it->second.type == RTypeProtocol::EntityType::BLOCK && position && position->x < -250) {
            if (_registry.isValid(*it->second.entity)) {
                _registry.removeEntity(*it->second.entity);
            }
            it = entities.erase(it);
        } else {
            ++it;
        }
    }
}

void GameMap::addBackground(std::shared_ptr<RealEngine::Entity> background,
                            RealEngine::ParallaxSystem&         parallaxSystem) {
    _backgroundEntities.emplace_back(background);
    parallaxSystem.update(_registry, 0.0f);  // this call is to set the background and the scale
}

void GameMap::synchroniseLevelBlockEntities() {
    for (auto& block : _blockEntities) {
        auto  posBlock = block.second;
        auto* position = _registry.getComponent<RealEngine::Position>(posBlock);
        if (position) {
            position->x += _xLevelPosition;
        }
    }
}
}  // namespace rtype