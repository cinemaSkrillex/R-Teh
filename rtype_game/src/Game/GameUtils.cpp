/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameUtils
*/

#include "Game/Game.hpp"

namespace rtype {

void Game::display_temporary_text(std::string text, sf::Vector2f position, sf::Color color,
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
    if (!_game_map) return;
    // _game_map->setLevelRunning(false);
    if (!_game_map->getMusicName().empty()) {
        RealEngine::AssetManager::getInstance().getMusic(_game_map->getMusicName())->stop();
    }

    // for (auto& background : _game_map->getBackgroundEntities()) {
    //     std::cout << "Removing background [" << *background << "]" << std::endl;
    //     if (_registry.is_valid(*background)) _registry.remove_entity(*background);
    // }
    // // _blockEntities.clear();
    // // _backgroundEntities.clear();
    // _game_map->setScrollingSpeed(0.0f);
    // _game_map->setXLevelPosition(0.0f);
    // _game_map->setIsMapLoaded(false);
    std::cout << "Clearing block entities" << std::endl;
}

void Game::relocateAllBlocks() {
    for (auto& [id, entityData] : _entities) {
        if (entityData.type == RTypeProtocol::EntityType::BLOCK) {
            std::cout << "Relocating block [" << *entityData.entity << "]" << std::endl;
            auto* positionComponent =
                _registry.get_component<RealEngine::Position>(entityData.entity);
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
