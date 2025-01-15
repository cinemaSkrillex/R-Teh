/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameUtils
*/

#include "Game/Game.hpp"

namespace rtype {

void Game::display_temporary_text(std::string text, sf::Vector2f position, sf::Color color) {
    auto tempText = std::make_shared<RealEngine::TemporaryText>(
        text, position, 1.0f, 0.5f, *(RealEngine::AssetManager::getInstance().getFont("pixel")));
    tempText->getText().setColor(color.r, color.g, color.b, color.a);
    tempText->getText().setRotation((rand() % 30) - 15);
    tempText->getText().move((rand() % 20) - 10, 0);
    _temporaryTexts.push_back(tempText);
}

}  // namespace rtype