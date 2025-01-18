/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RobotBossMinion
*/

#include "Game/Mobs/RobotBossMinion.hpp"

namespace rtype {

static void goForward(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime) {
    auto* position     = registry.get_component<RealEngine::Position>(entity);
    auto* velocity     = registry.get_component<RealEngine::Velocity>(entity);
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(entity);

    if (position && acceleration) {
        position->x += acceleration->ax * deltaTime;
        position->y += acceleration->ay * deltaTime;
    }
}

static void goInVerticalDirection(RealEngine::Registry& registry, RealEngine::Entity entity,
                                  float deltaTime) {
    auto* position     = registry.get_component<RealEngine::Position>(entity);
    auto* velocity     = registry.get_component<RealEngine::Velocity>(entity);
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(entity);

    if (position && acceleration) {
        position->y += acceleration->ay * deltaTime;
    }
}

static void alignOnTargetOnY(RealEngine::Registry& registry, RealEngine::Entity entity,
                             float deltaTime) {
    goForward(registry, entity, deltaTime);
    auto  entity_target  = registry.get_component<RealEngine::Target>(entity);
    auto* position       = registry.get_component<RealEngine::Position>(entity);
    auto* velocity       = registry.get_component<RealEngine::Velocity>(entity);
    auto* targetPosition = registry.get_component<RealEngine::Position>(entity_target->target);
    auto* acceleration   = registry.get_component<RealEngine::Acceleration>(entity);

    if (position && targetPosition) {
        float dy = targetPosition->y - position->y;

        if (std::abs(dy) > 1.0f) {
            velocity->vy = (dy > 0) ? std::min(acceleration->ay, dy / deltaTime)
                                    : std::max(-acceleration->ay, dy / deltaTime);
        } else {
            velocity->vy = 0;
        }
    }
}

RobotBossMinion::RobotBossMinion(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()),
      _mobSprite(*(RealEngine::AssetManager::getInstance().getSprite("robot_boss_minion"))) {
    _mobSpriteSheet.emplace("normal", _mobSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {130.f, 300.f}, 0.5f});
    registry.add_component(_entity, RealEngine::Acceleration{10.0f, 100.0f, 0.5f});

    registry.add_component(
        _entity,
        RealEngine::SpriteSheet{
            _mobSpriteSheet, "normal", 0, {32, 31}, false, true, 230, {-1, -1}, sf::Clock()});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});
    registry.add_component(_entity, RealEngine::AI{noBehavior, noBehavior, true});
    registry.add_component(_entity, RealEngine::Damage{15});
    registry.add_component(_entity, RealEngine::Health{30, 30});
    registry.add_component(_entity, RealEngine::TargetRadius{200.0f});
    registry.add_component(_entity, RealEngine::AI{alignOnTargetOnY, goForward, true});
    registry.add_component(_entity, RealEngine::Rotation{0.f});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
        _entity,
        RealEngine::NetvarContainer{{
            {"sprite_name", {"string", "sprite_name", std::string("robot_boss_minion"), nullptr}},
            {"new_entity", {"bool", "new_entity", true, nullptr}},
            {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}},
            {"powerup_drop", {"float", "powerup_drop", 50.f, nullptr}},
        }});
}

RobotBossMinion::~RobotBossMinion() {}

}  // namespace rtype