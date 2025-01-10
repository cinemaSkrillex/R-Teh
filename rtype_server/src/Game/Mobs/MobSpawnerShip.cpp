/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MobSpawnerShip
*/

#include "Game/Mobs/MobSpawnerShip.hpp"

namespace rtype {

MobSpawnerShip::MobSpawnerShip(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()),
      _mobSprite(*(RealEngine::AssetManager::getInstance().getSprite("mob_spawner_ship"))) {
    _mobSpriteSheet.emplace("normal", _mobSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {850.f, 850.f}, 0.5f});
    registry.add_component(
        _entity,
        RealEngine::SpriteSheet{
            _mobSpriteSheet, "normal", 0, {63, 50}, false, true, 135, {32, 25}, sf::Clock()});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});
    registry.add_component(_entity, RealEngine::AI{rushAndAimTowardsTarget, noBehavior, true});
    registry.add_component(_entity, RealEngine::Damage{50});
    registry.add_component(_entity, RealEngine::Health{40, 40});
    registry.add_component(_entity, RealEngine::Rotation{0.f});
    registry.add_component(
        _entity, RealEngine::NetvarContainer{
                     {{"sprite_name",
                       {"string", "sprite_name", std::string("mob_spawner_ship"), nullptr}}}});
}

MobSpawnerShip::~MobSpawnerShip() {}

}  // namespace rtype