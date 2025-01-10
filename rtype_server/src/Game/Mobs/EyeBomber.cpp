/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBomber
*/

#include "Game/Mobs/EyeBomber.hpp"

namespace rtype {

static void simpleBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                           float deltaTime) {
    auto* eyeRotation     = registry.get_component<RealEngine::Rotation>(entity);
    auto* eyeVelocity     = registry.get_component<RealEngine::Velocity>(entity);
    auto* eyeAcceleration = registry.get_component<RealEngine::Acceleration>(entity);
}

EyeBomber::EyeBomber(RealEngine::Registry& registry, sf::Vector2f position)
    : _eyeEntity(registry.spawn_entity()) {
    registry.add_component(_eyeEntity, RealEngine::Position{position.x, position.y});
    registry.add_component(_eyeEntity, RealEngine::Drawable{});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("eye_bomber");
    registry.add_component(_eyeEntity, RealEngine::SpriteSheet{spriteSheet});
    registry.add_component(_eyeEntity, RealEngine::Velocity{0.0f, 0.0f, {135.0f, 135.0f}, 0.8f});
    registry.add_component(_eyeEntity, RealEngine::Acceleration{60.0f, 30.0f, 0.5f});
    registry.add_component(_eyeEntity, RealEngine::Rotation{0.0f});
    registry.add_component(_eyeEntity,
                           RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});

    registry.add_component(_eyeEntity, RealEngine::Health{15, 50});
    registry.add_component(_eyeEntity,
                           RealEngine::AI{rushAndAimTowardsTargetZigzagging, simpleBehavior, true});
    registry.add_component(_eyeEntity, RealEngine::Damage{5});
    registry.add_component(
        _eyeEntity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("eye_bomber"), nullptr}}}});
}

EyeBomber::~EyeBomber() {}

void EyeBomber::setTarget(std::shared_ptr<RealEngine::Entity> target,
                          RealEngine::Registry&               registry) {
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(_eyeEntity);

    registry.add_component(_eyeEntity, RealEngine::Target{target});
}

}  // namespace rtype