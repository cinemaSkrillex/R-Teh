/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameUtils
*/

#include "Game/Game.hpp"

namespace rtype {

void Game::displayTemporaryText(std::string text, sf::Vector2f position, sf::Color color,
                                int size) {
    auto tempText = std::make_shared<RealEngine::TemporaryText>(
        text, position, 1.0f, 0.5f, RealEngine::AssetManager::getInstance().getFont("pixel"));
    tempText->getText().setColor(color.r, color.g, color.b, color.a);
    tempText->getText().setRotation((rand() % 30) - 15);
    tempText->getText().move((rand() % 20) - (rand() % 20), 0);
    tempText->getText().setCharacterSize(size);
    _temporaryTexts.push_back(tempText);
}

void Game::unloadLevel(float x, float y) {
    if (!_gameMap) return;
    // _gameMap->setLevelRunning(false);
    if (!_gameMap->getMusicName().empty()) {
        RealEngine::AssetManager::getInstance().getMusic(_gameMap->getMusicName())->stop();
    }

    // for (auto& background : _gameMap->getBackgroundEntities()) {
    //     std::cout << "Removing background [" << *background << "]" << std::endl;
    //     if (_registry.isValid(*background)) _registry.removeEntity(*background);
    // }
    // // _blockEntities.clear();
    // // _backgroundEntities.clear();
    // _gameMap->setScrollingSpeed(0.0f);
    // _gameMap->setXLevelPosition(0.0f);
    // _gameMap->setIsMapLoaded(false);
    std::cout << "Clearing block entities" << std::endl;
}

void Game::relocateAllBlocks() {
    for (auto& [id, entityData] : _entities) {
        if (entityData.type == RTypeProtocol::EntityType::BLOCK) {
            std::cout << "Relocating block [" << *entityData.entity << "]" << std::endl;
            auto* positionComponent =
                _registry.getComponent<RealEngine::Position>(entityData.entity);
            if (positionComponent) {
                positionComponent->x = 10000.0f;
                positionComponent->y = 10000.0f;
                std::cout << "Block [" << *entityData.entity << "] moved to (10000, 10000)"
                          << std::endl;
            }
        }
    }
}

}  // namespace rtype
