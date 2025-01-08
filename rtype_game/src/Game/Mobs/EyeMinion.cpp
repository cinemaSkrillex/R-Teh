/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBomber
*/

#include "Game/Mobs/EyeMinion.hpp"

namespace rtype {

static void simpleBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                           float deltaTime) {
    auto* eyeRotation     = registry.get_component<RealEngine::Rotation>(entity);
    auto* eyeVelocity     = registry.get_component<RealEngine::Velocity>(entity);
    auto* eyeAcceleration = registry.get_component<RealEngine::Acceleration>(entity);

    eyeVelocity->vx += eyeAcceleration->ax * deltaTime;
    eyeVelocity->vy += eyeAcceleration->ay * deltaTime;
    eyeRotation->angle = std::fmod(eyeRotation->angle + 360.0f, 360.0f);
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
            SmallLaser laser(registry, {position->x, position->y}, rotation->angle);
            cooldown = 3.5f;
        } else {
            cooldown = 1.5f;
        }
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

EyeMinion::EyeMinion(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
                     float speed)
    : _eyeEntity(registry.spawn_entity()) {
    std::cerr << "hello ???" << std::endl;
    //   _eyeSprite(*(RealEngine::AssetManager::getInstance().getSprite("eye_minion"))) {
    // _eyeSheet.emplace("normal", _eyeSprite);

    registry.add_component(_eyeEntity, RealEngine::Position{position.x, position.y});
    registry.add_component(_eyeEntity, RealEngine::Drawable{});
    //get the spritesheet from the asset manager getSpriteSheet
    std::cerr << "hello" << std::endl;
    auto eyeSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("eye_minion");
    std::cerr << "eyeSheet: " << eyeSheet.sprites.size() << std::endl;
    // registry.add_component(_eyeEntity, eyeSheet);
    // registry.add_components(
    //     _eyeEntity,
    //     RealEngine::SpriteSheet{
    //         _eyeSheet, "normal", 0, {18, 11}, false, true, 120, {14, 5}, sf::Clock()},
    //     RealEngine::Drawable{});
    registry.add_component(_eyeEntity, RealEngine::Velocity{0.0f, 0.0f, {135.0f, 135.0f}, 0.8f});
    registry.add_component(_eyeEntity, RealEngine::Acceleration{60.0f, 30.0f, 0.5f});
    registry.add_component(_eyeEntity, RealEngine::Rotation{0.0f});
    registry.add_component(_eyeEntity,
                           RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});

    registry.add_component(_eyeEntity, RealEngine::Health{50, 50});
    registry.add_component(_eyeEntity,
                           RealEngine::AI{rushAndAimTowardsTarget, simpleBehavior, true});
    registry.add_component(_eyeEntity, RealEngine::Damage{5});
    registry.add_component(
        _eyeEntity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("eye_minion"), nullptr}},
             {"shootCooldown", {"float", "shootCooldown", 3.5f, updateShootCooldown}}}});
}

EyeMinion::~EyeMinion() {}

void EyeMinion::setTarget(std::shared_ptr<RealEngine::Entity> target,
                          RealEngine::Registry&               registry) {
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(_eyeEntity);

    acceleration->ax = 240.0f;
    acceleration->ay = 240.0f;
    registry.add_component(_eyeEntity, RealEngine::Target{target});
}

}  // namespace rtype