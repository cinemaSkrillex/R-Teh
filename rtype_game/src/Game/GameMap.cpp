/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameMap
*/

// #include "Game/GameMap.hpp"

#include "Game.hpp"

namespace rtype {

GameMap::GameMap(RealEngine::Registry& registry, Game* game)
    : _registry(registry), _game(game), _levelRunning(false) {}

GameMap::~GameMap() { std::cout << "GameMap destroyed" << std::endl; }

void GameMap::updateLevel(float deltaTime) {
    if (!_levelRunning) {
        return;
    }
    x_level_position += _scrollingSpeed * deltaTime;
    for (auto& block : _blockEntities) {
        if (!block.second) {
            std::cout << "Block is null" << std::endl;
            continue;
        }
        auto* position = _registry.get_component<RealEngine::Position>(block.second);
        if (position) {
            position->x -= _scrollingSpeed * deltaTime;
        }
    }
    removeDeadBlocks();
}

void GameMap::startLevel() {
    _levelRunning = true;
    if (!_music_name.empty()) {
        RealEngine::AssetManager::getInstance().getMusic(_music_name)->play();
    }
}

void GameMap::stopLevel() {
    _levelRunning = false;
    if (!_music_name.empty()) {
        RealEngine::AssetManager::getInstance().getMusic(_music_name)->stop();
    }
}

void GameMap::unloadLevel() {
    _levelRunning = false;
    if (!_music_name.empty()) {
        RealEngine::AssetManager::getInstance().getMusic(_music_name)->stop();
    }
    // for (auto block : _blockEntities) {
    //     std::cout << "Removing block" << std::endl;
    //     if (block.second) {
    //         std::cout << "Block is not null" << std::endl;
    //         _registry.add_component(block.second, RealEngine::AutoDestructible{0.0f});
    //     }
    // }
    // for (auto& entity : _game->getEntitiesGame()) {
    //     if (std::find_if(
    //             _blockEntities.begin(), _blockEntities.end(),
    //             [&entity](const std::pair<long int, std::shared_ptr<RealEngine::Entity>>& block)
    //             {
    //                 return block.first == entity.first;
    //             }) != _blockEntities.end()) {
    //         std::cout << "Removing block [" << *entity.second << "]" << std::endl;
    //         _registry.remove_entity(*entity.second);
    //     }
    // }
    for (auto& block : _blockEntities) {
        if (block.second) {
            std::cout << "Removing block [" << *block.second << "]" << std::endl;
            _registry.remove_entity(*block.second);
        }
    }
    for (auto& background : _backgroundEntities) {
        std::cout << "Removing background [" << *background << "]" << std::endl;
        _registry.remove_entity(*background);
    }
    // _blockEntities.clear();
    // _backgroundEntities.clear();
    _scrollingSpeed  = 0.0f;
    x_level_position = 0.0f;
    _isMapLoaded     = false;
    std::cout << "Clearing block entities" << std::endl;
}

void GameMap::removeDeadBlocks() {
    _blockEntities.erase(
        std::remove_if(_blockEntities.begin(), _blockEntities.end(),
                       [](const std::pair<long int, std::shared_ptr<RealEngine::Entity>>& block) {
                           return block.second == nullptr;
                       }),
        _blockEntities.end());
}

void GameMap::addBackground(std::shared_ptr<RealEngine::Entity> background,
                            RealEngine::ParallaxSystem&         parallaxSystem) {
    _backgroundEntities.emplace_back(background);
    parallaxSystem.update(_registry, 0.0f);  // this call is to set the background and the scale
}

void GameMap::synchroniseLevelBlockEntities() {
    for (auto& block : _blockEntities) {
        auto  posBlock = block.second;
        auto* position = _registry.get_component<RealEngine::Position>(posBlock);
        if (position) {
            position->x += x_level_position;
        }
    }
}
}  // namespace rtype