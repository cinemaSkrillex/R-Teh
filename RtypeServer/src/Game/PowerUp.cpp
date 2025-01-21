/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** PowerUp
*/

#include "Game/PowerUp.hpp"

namespace rtype {

static void collisionBehavior(RealEngine::CollisionType collisionType,
                              RealEngine::Registry& registry, RealEngine::Entity collider,
                              RealEngine::Entity entity) {
    destroyOutOfScreen(collisionType, registry, entity);
    switch (collisionType) {
        case RealEngine::CollisionType::PLAYER:
            selfDestruct(registry, entity);
            break;
        default:
            break;
    }
}

PowerUp::PowerUp(RealEngine::Registry& registry, sf::Vector2f pos, bonusType type)
    : _entity(registry.spawnEntity()) {
    std::string sprite_name = "";
    switch (type) {
        case HEAL:
            sprite_name = "heal_powerup";
            break;
        case SHOOT:
            sprite_name = "shoot_powerup";
            break;
        case SPEED:
            sprite_name = "speed_powerup";
            break;
        default:
            break;
    }
    registry.addComponent(_entity, RealEngine::Position{pos.x, pos.y});
    registry.addComponent(_entity, RealEngine::Velocity{0.0f, 0.0f, {1000.0f, 1000.0f}, 2.5f});
    registry.addComponent(_entity, RealEngine::Acceleration{5.0f, 5.0f, 0.0f});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet(sprite_name);
    registry.addComponent(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.addComponent(_entity, RealEngine::Collision{{0, 0, 16, 16},
                                                         "powerup",
                                                         false,
                                                         RealEngine::CollisionType::PICKABLE,
                                                         collisionBehavior});
    registry.addComponent(_entity, RealEngine::TargetRadius{150, false});
    if (type == HEAL) {
        registry.addComponent(_entity, RealEngine::AutoDestructible{10});
    }
    registry.addComponent(_entity, RealEngine::AI{rushTowardsTarget, noBehavior, true});
    int score = 0;
    switch (type) {
        case HEAL:
            score = 20;
            break;
        case SHOOT:
            score = 100;
            break;
        case SPEED:
            score = 150;
            break;
        default:
            break;
    }
    registry.addComponent(_entity, RealEngine::Score{score});
    registry.addComponent(
        _entity, RealEngine::NetvarContainer{
                     {{"sprite_name", {"string", "sprite_name", sprite_name, nullptr}},
                      {"new_entity", {"bool", "new_entity", true, nullptr}},
                      {"powerup_type", {"int", "type", static_cast<int>(type), nullptr}}}});
}

PowerUp::~PowerUp() {}

}  // namespace rtype