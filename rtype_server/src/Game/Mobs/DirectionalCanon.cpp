/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** DirectionalCanon
*/

#include "Game/Mobs/DirectionalCanon.hpp"

namespace rtype {

DirectionalCanon::DirectionalCanon(RealEngine::Registry& registry, sf::Vector2f position,
                                   float level_speed)
    : _entity(registry.spawn_entity()),
      _mobSprite(*(RealEngine::AssetManager::getInstance().getSprite("directional_canon"))) {
    _mobSpriteSheet.emplace("normal", _mobSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Acceleration{level_speed, 0, 0});
    registry.add_component(
        _entity,
        RealEngine::SpriteSheet{
            _mobSpriteSheet, "normal", 0, {64, 64}, false, true, 80, {32, 32}, sf::Clock()});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});
    registry.add_component(_entity, RealEngine::AI{noBehavior, moveWithoutVelocity, true});
    registry.add_component(_entity, RealEngine::Damage{50});
    registry.add_component(_entity, RealEngine::Health{40, 40});
    registry.add_component(_entity, RealEngine::Rotation{0.f});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});

    registry.add_component(
        _entity, RealEngine::NetvarContainer{
                     {{"sprite_name",
                       {"string", "sprite_name", std::string("directional_canon"), nullptr}}}});
}

DirectionalCanon::~DirectionalCanon() {}

}  // namespace rtype