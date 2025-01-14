/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Background
*/

#include "Game/Background.hpp"

namespace rtype {
Background::Background(RealEngine::Registry& registry, float speed, std::string sprite_str)
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::Position{0.f, 0.f});
    registry.add_component(
        _entity, RealEngine::SpriteComponent{
                     *(RealEngine::AssetManager::getInstance().getSprite(sprite_str)), -1});
    registry.add_component(_entity, RealEngine::Parallax{speed, sf::Vector2f(800.0f, 600.0f)});
}

Background::~Background() {}

}  // namespace rtype