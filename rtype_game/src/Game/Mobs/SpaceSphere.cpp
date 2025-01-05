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
    auto* position                           = registry.get_component<RealEngine::Position>(entity);
    auto* velocity                           = registry.get_component<RealEngine::Velocity>(entity);
    std::vector<RealEngine::Netvar*> netvars = registry.get_components<RealEngine::Netvar>(entity);
    for (auto& netvar : netvars) {
        std::cout << "netvar name: " << netvar->name << std::endl;
        if (netvar->name == "shootCooldown") {
            float cooldown = std::any_cast<float>(netvar->value);
            std::cout << "cooldown: " << cooldown << std::endl;
            if (cooldown <= 0) {
                Fireball fireball(registry, {position->x, position->y}, 180, 200);
                netvar->value = 0.5f;
            }
        }
    }
}

static void updateCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                           float deltaTime) {
    std::vector<RealEngine::Netvar*> netvars = registry.get_components<RealEngine::Netvar>(entity);
    for (auto& netvar : netvars) {
        std::cout << "netvar name: " << netvar->name << std::endl;
        if (netvar->name != "shootCooldown") {
            continue;
        }
        float cooldown = std::any_cast<float>(netvar->value);
        cooldown -= deltaTime;
        if (cooldown < 0) {
            cooldown = 0;
        }
        netvar->value = cooldown;
    }
}

SpaceSphere::SpaceSphere(RealEngine::Registry& registry, sf::Vector2f position,
                         sf::Vector2f direction, float speed)
    : _entity(registry.spawn_entity()),
      _mobSprite(*(RealEngine::AssetManager::getInstance().getSprite("space_sphere"))) {
    _mobSpriteSheet.emplace("normal", _mobSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{speed, 0, {850.f, 850.f}, 0.5f});
    registry.add_component(
        _entity, RealEngine::SpriteSheet{
                     _mobSpriteSheet, "normal", 0, {16, 14}, false, true, 55, {8, 8}, sf::Clock()});
    // 55 is the speed of rotation animation
    // registry.add_component(_entity, RealEngine::SpriteComponent{_mobSprite});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 noCollisionBehavior});
    registry.add_component(_entity, RealEngine::AI{noBehavior, straight_line_behavior, true});
    registry.add_component(_entity, RealEngine::Damage{50});
    registry.add_component(_entity, RealEngine::Health{40, 40});
    registry.add_component(_entity, RealEngine::Rotation{0.f});
    registry.add_component(_entity,
                           RealEngine::Netvar{"MOB", "shootCooldown", 0.5f, updateCooldown});
    // registry.add_component(_entity,
    //                        RealEngine::Netvar{"MOB", "changeDirection", 2.0f, updateCooldown});
}

SpaceSphere::~SpaceSphere() {}

}  // namespace rtype