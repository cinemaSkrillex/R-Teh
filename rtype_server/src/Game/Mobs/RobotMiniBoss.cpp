/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RobotMiniBoss
*/

#include "Game/Mobs/RobotMiniBoss.hpp"

namespace rtype {

static void straight_line_behavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                                   float deltaTime) {
    auto* position = registry.get_component<RealEngine::Position>(entity);
    auto* velocity = registry.get_component<RealEngine::Velocity>(entity);

    if (position->x > 0) {
        velocity->vx = -100.0f;
    }
}

RobotMiniBoss::RobotMiniBoss(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()),
      _shootMobSprite(*(RealEngine::AssetManager::getInstance().getSprite("robot_boss_shoot"))),
      _fordwardMobSprite(
          *(RealEngine::AssetManager::getInstance().getSprite("robot_boss_fordward"))),
      _backwardMobSprite(
          *(RealEngine::AssetManager::getInstance().getSprite("robot_boss_backward"))) {
    _mobSpriteSheet.emplace("shoot", _shootMobSprite);
    _mobSpriteSheet.emplace("fordward", _fordwardMobSprite);
    _mobSpriteSheet.emplace("backwards", _backwardMobSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {500.f, 500.f}, 0.5f});
    registry.add_component(
        _entity,
        RealEngine::SpriteSheet{
            _mobSpriteSheet, "shoot", 0, {47, 43}, false, false, 0, {-1, -1}, sf::Clock()});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});
    registry.add_component(_entity, RealEngine::AI{noBehavior, noBehavior, true});
    registry.add_component(_entity, RealEngine::Damage{50});
    registry.add_component(_entity, RealEngine::Health{40, 40});
    registry.add_component(_entity, RealEngine::Rotation{0.f});
    registry.add_component(
        _entity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("robot_mini_boss"), nullptr}}}});
}

RobotMiniBoss::~RobotMiniBoss() {}

}  // namespace rtype