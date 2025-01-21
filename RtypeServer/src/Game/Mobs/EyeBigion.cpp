/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBigion
*/

#include "Game/Mobs/EyeBigion.hpp"

namespace rtype {

static void makeEyeSpawn(RealEngine::Registry& registry, RealEngine::Entity entity,
                         RealEngine::Netvar& currentNetvar, float deltaTime) {
    float cooldown = std::any_cast<float>(currentNetvar.value);
    auto* position = registry.getComponent<RealEngine::Position>(entity);

    if (cooldown <= 0 && position) {
        if (rand() % 2 == 0) {
            EyeBomber eyeBomber(registry, {position->x, position->y});
        }
        cooldown = 5.0f;
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

static void simpleBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                           float deltaTime) {
    registry.addComponent(entity, RealEngine::Acceleration{-20.0f, 0.0f, 0.5f});
    goStraightConstant(registry, entity, deltaTime);
}

static void adjustSpeedAndRush(RealEngine::Registry& registry, RealEngine::Entity entity,
                               float deltaTime) {
    registry.addComponent(entity, RealEngine::Acceleration{40.0f, 40.0f, 0.5f});
    rushAndAimTowardsTarget(registry, entity, deltaTime);
}

EyeBigion::EyeBigion(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("eye_bigion");
    registry.addComponent(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.addComponent(_entity, RealEngine::Velocity{0.0f, 0.0f, {100.0f, 100.0f}, 0.8f});
    registry.addComponent(_entity, RealEngine::Acceleration{40.0f, 40.0f, 0.5f});
    registry.addComponent(_entity, RealEngine::Rotation{180.0f});
    registry.addComponent(_entity,
                          RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                "mob",
                                                false,
                                                RealEngine::CollisionType::ENEMY,
                                                takesDamage});
    registry.addComponent(_entity, RealEngine::Health{80, 80});
    registry.addComponent(_entity, RealEngine::AI{adjustSpeedAndRush, simpleBehavior, true});
    registry.addComponent(_entity, RealEngine::Damage{20});
    registry.addComponent(_entity, RealEngine::TargetRadius{300.0f});
    registry.addComponent(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.addComponent(
        _entity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("eye_bigion"), nullptr}},
             {"new_entity", {"bool", "new_entity", true, nullptr}},
             {"entity_type", {"int", "entity_type", 2, nullptr}},
             {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}},
             {"eye_spawn_cooldown", {"float", "eye_spawn_cooldown", 10.0f, makeEyeSpawn}},
             {"powerup_drop", {"float", "powerup_drop", 30.f, nullptr}},
             {"powerup_type", {"int", "powerup_type", 2, nullptr}}}});
    registry.addComponent(_entity, RealEngine::Score{120});
}

EyeBigion::~EyeBigion() {}

void EyeBigion::setTarget(std::shared_ptr<RealEngine::Entity> target,
                          RealEngine::Registry&               registry) {
    registry.addComponent(_entity, RealEngine::Target{target});
}

}  // namespace rtype