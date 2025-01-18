/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RobotMiniBoss
*/

#include "Game/Mobs/RobotMiniBoss.hpp"

namespace rtype {

static void goUpAndDown(RealEngine::Registry& registry, RealEngine::Entity entity,
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

static void updateShootCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                                RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* target = registry.get_component<RealEngine::Target>(entity);
    if (!target) return;

    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.get_component<RealEngine::Position>(entity);
    auto* rotation  = registry.get_component<RealEngine::Rotation>(entity);
    float cooldown  = std::any_cast<float>(currentNetvar.value);

    if (cooldown <= 0) {
        if (rand() % 2 == 0) {
            MiniBossShoot shoot(registry, {position->x, position->y}, rotation->angle, 1);
            cooldown = 3.5f;
        } else {
            cooldown = 1.5f;
        }
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

static void updateVortexCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                                 RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* target = registry.get_component<RealEngine::Target>(entity);
    if (!target) return;

    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.get_component<RealEngine::Position>(entity);
    auto* rotation  = registry.get_component<RealEngine::Rotation>(entity);
    float cooldown  = std::any_cast<float>(currentNetvar.value);

    if (cooldown <= 0) {
        SpaceVortex vortex(registry, {position->x, position->y});
        cooldown = 5.0f;
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

static void updateHorizontalMoveCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                                         RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* target = registry.get_component<RealEngine::Target>(entity);
    if (!target) return;

    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.get_component<RealEngine::Position>(entity);
    auto* rotation  = registry.get_component<RealEngine::Rotation>(entity);
    float cooldown  = std::any_cast<float>(currentNetvar.value);

    if (cooldown <= 0) {
        if (rand() % 2 == 0) {
            container->getNetvar("isFordward")->value = true;
        } else {
            container->getNetvar("isFordward")->value = false;
        }
        cooldown = 6.4f;
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

static void updateMinionsCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                                  RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* target = registry.get_component<RealEngine::Target>(entity);
    if (!target) return;

    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.get_component<RealEngine::Position>(entity);
    auto* rotation  = registry.get_component<RealEngine::Rotation>(entity);
    float cooldown  = std::any_cast<float>(currentNetvar.value);

    if (cooldown <= 0) {
        RobotBossMinion minion(registry, {position->x, position->y});
        cooldown = 15.0f;
        std::cout << "Minion spawned" << std::endl;
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
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
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {50.f, 200.f}, 0.5f});
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
    registry.add_component(_entity, RealEngine::AI{alignOnTargetOnY, noBehavior, true});
    registry.add_component(_entity, RealEngine::Damage{50});
    registry.add_component(_entity, RealEngine::Health{450, 450});
    registry.add_component(_entity, RealEngine::Rotation{0.f});
    registry.add_component(_entity, RealEngine::Damage{25});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
        _entity,
        RealEngine::NetvarContainer{{
            {"sprite_name", {"string", "sprite_name", std::string("robot_mini_boss"), nullptr}},
            {"new_entity", {"bool", "new_entity", true, nullptr}},
            {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}},
            {"shootCooldown", {"float", "shootCooldown", 2.5f, updateShootCooldown}},
            {"vortexCooldown", {"float", "vortexCooldown", 5.0f, updateVortexCooldown}},
            {"horizontalMoveCooldown",
             {"float", "horizontalMoveCooldown", 6.4f, updateHorizontalMoveCooldown}},
            {"isFordward", {"bool", "isFordward", false, nullptr}},
            {"verticalMoveCooldown", {"float", "verticalMoveCooldown", 2.5f, updateVortexCooldown}},
            {"minionsCooldown", {"float", "minionsCooldown", 5.0f, updateMinionsCooldown}},
        }});
    registry.add_component(_entity, RealEngine::Score{500});
}

RobotMiniBoss::~RobotMiniBoss() {}

}  // namespace rtype