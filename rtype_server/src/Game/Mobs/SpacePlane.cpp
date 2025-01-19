/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpacePlane
*/

#include "Game/Mobs/SpacePlane.hpp"

namespace rtype {

static void updateShootCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                                RealEngine::Netvar& currentNetvar, float deltaTime) {

    if (!registry.is_valid(entity)) {
        std::cerr << "Entity is no longer valid: " << entity << std::endl;
        return;
    }
    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.get_component<RealEngine::Position>(entity);
    auto* rotation  = registry.get_component<RealEngine::Rotation>(entity);
    if (!currentNetvar.value.has_value()) {
        std::cerr << "Netvar value is empty for entity: " << entity << std::endl;
        return;
    }
    float cooldown  = std::any_cast<float>(currentNetvar.value);

    if (!container) {
        std::cerr << "Invalid container component for entity: " << entity << std::endl;
        return;
    }
    if (!position || !rotation) {
        std::cerr << "Invalid position or rotation component for entity: " << entity << std::endl;
        return;
    }

    if (cooldown <= 0) {
        if (rand() % 3 <= 1) {
            SimpleShoot shoot(registry, {position->x, position->y}, 90);
            // SimpleShoot shoot2(registry, {position->x, position->y}, 270);
            cooldown = 2.5f;
        } else {
            cooldown = 1.5f;
        }
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

SpacePlane::SpacePlane(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(
        _entity, RealEngine::Interpolation{
                     {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {140.f, 140.f}, 0.5f});
    registry.add_component(_entity, RealEngine::Acceleration{-100.f, 0.f, 0.5f});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("space_plane");
    registry.add_component(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});
    registry.add_component(_entity, RealEngine::AI{noBehavior, goStraightConstant, true});
    registry.add_component(_entity, RealEngine::Damage{10});
    registry.add_component(_entity, RealEngine::Health{20, 20});
    registry.add_component(_entity, RealEngine::Rotation{0.f});
    registry.add_component(_entity, RealEngine::Score{20});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
        _entity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("space_plane"), nullptr}},
             {"new_entity", {"bool", "new_entity", true, nullptr}},
             {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}},
             {"entity_type", {"int", "entity_type", 2, nullptr}},
             {"shootCooldown", {"float", "shootCooldown", 2.5f, updateShootCooldown}},
             {"powerup_drop", {"float", "powerup_drop", 5.f, nullptr}}}});
}

SpacePlane::~SpacePlane() {}

}  // namespace rtype