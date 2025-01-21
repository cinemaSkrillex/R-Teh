/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MobSpawnerShip
*/

#include "Game/Mobs/MobSpawnerShip.hpp"

namespace rtype {

static void makeEntitySpawn(RealEngine::Registry& registry, RealEngine::Entity entity,
                            RealEngine::Netvar& currentNetvar, float deltaTime) {
    float cooldown = std::any_cast<float>(currentNetvar.value);
    auto* position = registry.getComponent<RealEngine::Position>(entity);

    if (cooldown <= 0) {
        if (rand() % 2 == 0) {
            SpacePlane plane(registry, {position->x, position->y});
        }
        cooldown = 5.0f;
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

MobSpawnerShip::MobSpawnerShip(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.addComponent(_entity, RealEngine::Velocity{0, 0, {850.f, 850.f}, 0.5f});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("mob_spawner_ship");
    registry.addComponent(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.addComponent(_entity,
                          RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                "mob",
                                                false,
                                                RealEngine::CollisionType::ENEMY,
                                                takesDamage});
    registry.addComponent(_entity, RealEngine::AI{rushAndAimTowardsTarget, noBehavior, true});
    registry.addComponent(_entity, RealEngine::Damage{10});
    registry.addComponent(_entity, RealEngine::Health{100, 100});
    registry.addComponent(_entity, RealEngine::Rotation{0.f});
    registry.addComponent(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.addComponent(
        _entity,
        RealEngine::NetvarContainer{{
            {"sprite_name", {"string", "sprite_name", std::string("mob_spawner_ship"), nullptr}},
            {"new_entity", {"bool", "new_entity", true, nullptr}},
            {"entity_type", {"int", "entity_type", 2, nullptr}},
            {"make_enemy_spawn", {"float", "make_enemy_spawn", 5.0f, makeEntitySpawn}},
        }});
}

MobSpawnerShip::~MobSpawnerShip() {}

}  // namespace rtype