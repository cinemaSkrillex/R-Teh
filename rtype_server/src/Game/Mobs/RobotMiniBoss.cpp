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
    auto* position     = registry.getComponent<RealEngine::Position>(entity);
    auto* velocity     = registry.getComponent<RealEngine::Velocity>(entity);
    auto* acceleration = registry.getComponent<RealEngine::Acceleration>(entity);

    if (position && acceleration) {
        position->y += acceleration->ay * deltaTime;
    }
}

static void alignOnTargetOnY(RealEngine::Registry& registry, RealEngine::Entity entity,
                             float deltaTime) {
    auto  entity_target  = registry.getComponent<RealEngine::Target>(entity);
    auto* position       = registry.getComponent<RealEngine::Position>(entity);
    auto* velocity       = registry.getComponent<RealEngine::Velocity>(entity);
    auto* targetPosition = registry.getComponent<RealEngine::Position>(entity_target->target);
    auto* acceleration   = registry.getComponent<RealEngine::Acceleration>(entity);

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
    auto* target = registry.getComponent<RealEngine::Target>(entity);
    if (!target) return;

    auto* container = registry.getComponent<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.getComponent<RealEngine::Position>(entity);
    auto* rotation  = registry.getComponent<RealEngine::Rotation>(entity);
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
    auto* target = registry.getComponent<RealEngine::Target>(entity);
    if (!target) return;

    auto* container = registry.getComponent<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.getComponent<RealEngine::Position>(entity);
    auto* rotation  = registry.getComponent<RealEngine::Rotation>(entity);
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
    auto* target = registry.getComponent<RealEngine::Target>(entity);
    if (!target) return;

    auto* container = registry.getComponent<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.getComponent<RealEngine::Position>(entity);
    auto* rotation  = registry.getComponent<RealEngine::Rotation>(entity);
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
    auto* target = registry.getComponent<RealEngine::Target>(entity);
    if (!target) return;

    auto* container = registry.getComponent<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.getComponent<RealEngine::Position>(entity);
    auto* rotation  = registry.getComponent<RealEngine::Rotation>(entity);
    float cooldown  = std::any_cast<float>(currentNetvar.value);

    if (cooldown <= 0) {
        RobotBossMinion minion(registry, {800.f, 600.f});
        RobotBossMinion minion2(registry, {800.f, 0.f});
        cooldown = 15.0f;
        std::cout << "Minion spawned" << std::endl;
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

RobotMiniBoss::RobotMiniBoss(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.addComponent(_entity, RealEngine::Velocity{0, 0, {50.f, 200.f}, 0.5f});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("robot_mini_boss");
    registry.addComponent(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.addComponent(_entity, RealEngine::Drawable{});
    registry.addComponent(_entity,
                          RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                "mob",
                                                false,
                                                RealEngine::CollisionType::ENEMY,
                                                takesDamage});
    registry.addComponent(_entity, RealEngine::AI{alignOnTargetOnY, noBehavior, true});
    registry.addComponent(_entity, RealEngine::Damage{50});
    registry.addComponent(_entity, RealEngine::Health{850, 850});
    registry.addComponent(_entity, RealEngine::Rotation{0.f});
    registry.addComponent(_entity, RealEngine::Damage{25});
    registry.addComponent(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.addComponent(
        _entity,
        RealEngine::NetvarContainer{{
            {"sprite_name", {"string", "sprite_name", std::string("robot_mini_boss"), nullptr}},
            {"new_entity", {"bool", "new_entity", true, nullptr}},
            {"entity_type", {"int", "entity_type", 2, nullptr}},
            {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}},
            {"shootCooldown", {"float", "shootCooldown", 2.5f, updateShootCooldown}},
            {"vortexCooldown", {"float", "vortexCooldown", 5.0f, updateVortexCooldown}},
            {"horizontalMoveCooldown",
             {"float", "horizontalMoveCooldown", 6.4f, updateHorizontalMoveCooldown}},
            {"isFordward", {"bool", "isFordward", false, nullptr}},
            {"verticalMoveCooldown", {"float", "verticalMoveCooldown", 2.5f, updateVortexCooldown}},
            {"minionsCooldown", {"float", "minionsCooldown", 5.0f, updateMinionsCooldown}},
        }});
    registry.addComponent(_entity, RealEngine::Score{500});
}

RobotMiniBoss::~RobotMiniBoss() {}

}  // namespace rtype