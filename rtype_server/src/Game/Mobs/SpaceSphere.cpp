/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpaceSphere
*/

#include "Game/Mobs/SpaceSphere.hpp"

namespace rtype {

static void straight_line_behavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                                   float deltaTime) {
    auto* position     = registry.get_component<RealEngine::Position>(entity);
    auto* velocity     = registry.get_component<RealEngine::Velocity>(entity);
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(entity);
    auto* container    = registry.get_component<RealEngine::NetvarContainer>(entity);

    if (container != nullptr) {
        float shootCooldown = std::any_cast<float>(container->getNetvar("shootCooldown")->value);
        // if (shootCooldown <= 0) {
        //     Fireball fireball(registry, {position->x, position->y}, 180);
        // }
        bool goUp = std::any_cast<bool>(container->getNetvar("goUp")->value);
        if (goUp) {
            velocity->vy = -acceleration->ay * deltaTime;
        } else {
            velocity->vy = acceleration->ay * deltaTime;
        }
    }
}

static void updateShootCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                                RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
    float cooldown  = std::any_cast<float>(currentNetvar.value);

    if (cooldown <= 0) {
        cooldown = 0.5f;
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

static void updateDirectionCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                                    RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
    float cooldown  = std::any_cast<float>(currentNetvar.value);

    if (cooldown <= 0) {
        cooldown = 2.0f;
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

static void updateDirection(RealEngine::Registry& registry, RealEngine::Entity entity,
                            RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* container         = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto* velocity          = registry.get_component<RealEngine::Velocity>(entity);
    auto* cooldownNetvar    = container->getNetvar("directionCooldown");
    float directionCooldown = std::any_cast<float>(cooldownNetvar->value);
    bool  goUp              = std::any_cast<bool>(currentNetvar.value);

    if (directionCooldown <= 0) {
        velocity->vy = 0;
        goUp         = !goUp;
    }
    currentNetvar.value = goUp;
}

SpaceSphere::SpaceSphere(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(
        _entity, RealEngine::Interpolation{
                     {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});

    registry.add_component(_entity, RealEngine::Velocity{0, 0, {850.f, 850.f}, 0.5f});
    registry.add_component(_entity, RealEngine::Acceleration{1000.0f, 1000.0f, 0.5f});

    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("space_sphere");
    registry.add_component(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});
    registry.add_component(_entity, RealEngine::AI{noBehavior, straight_line_behavior, true});
    registry.add_component(_entity, RealEngine::Damage{10});
    registry.add_component(_entity, RealEngine::Health{30, 30});
    registry.add_component(_entity, RealEngine::Rotation{0.f});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
        _entity,
        RealEngine::NetvarContainer{{
            {"sprite_name", {"string", "sprite_name", std::string("space_sphere"), nullptr}},
            {"shootCooldown", {"float", "shootCooldown", 0.5f, updateShootCooldown}},
            {"directionCooldown", {"float", "directionCooldown", 2.0f, updateDirectionCooldown}},
            {"goUp", {"bool", "goUp", false, updateDirection}},
            {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}},
            {"new_entity", {"bool", "new_entity", true, nullptr}},
            {"powerup_drop", {"float", "powerup_drop", 70.f, nullptr}},
        }});
    registry.add_component(_entity, RealEngine::Score{75});
}

SpaceSphere::~SpaceSphere() {}

}  // namespace rtype