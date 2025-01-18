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
    auto* position = registry.get_component<RealEngine::Position>(entity);

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
    : _entity(registry.spawn_entity()),
      _mobSprite(*(RealEngine::AssetManager::getInstance().getSprite("mob_spawner_ship"))) {
    _mobSpriteSheet.emplace("normal", _mobSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {850.f, 850.f}, 0.5f});
    registry.add_component(
        _entity,
        RealEngine::SpriteSheet{
            _mobSpriteSheet, "normal", 0, {63, 50}, false, true, 135, {32, 25}, sf::Clock()});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});
    registry.add_component(_entity, RealEngine::AI{rushAndAimTowardsTarget, noBehavior, true});
    registry.add_component(_entity, RealEngine::Damage{10});
    registry.add_component(_entity, RealEngine::Health{100, 100});
    registry.add_component(_entity, RealEngine::Rotation{0.f});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
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