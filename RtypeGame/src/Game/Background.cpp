/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Background
*/

#include "Game/Background.hpp"

namespace rtype {
Background::Background(RealEngine::Registry& registry, float speed, std::string sprite_str)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::Position{0.f, 0.f});
    registry.addComponent(
        _entity, RealEngine::SpriteComponent{
                     *(RealEngine::AssetManager::getInstance().getSprite(sprite_str)), -1});
    registry.addComponent(_entity, RealEngine::Parallax{speed, sf::Vector2f(800.0f, 600.0f)});
}

Background::~Background() {}

}  // namespace rtype