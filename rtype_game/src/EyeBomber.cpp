#include "EyeBomber.hpp"

namespace rtype {
EyeBomber::EyeBomber(RealEngine::Registry& registry, sf::Vector2f position)
    : _eyeEntity(registry.spawn_entity()),
      _eyeSprite("../../assets/sprites/the_eye/bomber.png", sf::IntRect{0, 0, 15 * 2, 10}),
      _directionTimer(0.0f) {
    _eyeSprite.setScale(GAME_SCALE, GAME_SCALE);
    _eyeSheet.emplace("normal", _eyeSprite);

    registry.add_component(_eyeEntity, RealEngine::Position{position.x, position.y});
    registry.add_components(
        _eyeEntity,
        RealEngine::SpriteSheet{_eyeSheet, "normal", 0, {15, 10}, false, true, 120, {10, 5}},
        RealEngine::Drawable{});
    registry.add_component(_eyeEntity, RealEngine::Velocity{0.0f, 0.0f, {135.0f, 135.0f}, 0.8f});
    registry.add_component(_eyeEntity, RealEngine::Acceleration{240.0f, 240.0f, 2.0f});
    registry.add_component(_eyeEntity, RealEngine::Rotation{0.0f});
    registry.add_component(
        _eyeEntity,
        RealEngine::AI{[this](RealEngine::Registry& registry, RealEngine::Entity target,
                              float deltaTime) { agressiveBehavior(registry, target, deltaTime); },
                       [this](RealEngine::Registry& registry, float deltaTime) {
                           simpleBehavior(registry, deltaTime);
                       },
                       true});
}

EyeBomber::~EyeBomber() {}

void EyeBomber::setTarget(RealEngine::Entity target, RealEngine::Registry& registry) {
    registry.add_component(_eyeEntity, RealEngine::Target{target});
}

void EyeBomber::agressiveBehavior(RealEngine::Registry& registry, RealEngine::Entity target,
                                  float deltaTime) {
    auto* position       = registry.get_component<RealEngine::Position>(_eyeEntity);
    auto* acceleration   = registry.get_component<RealEngine::Acceleration>(_eyeEntity);
    auto* velocity       = registry.get_component<RealEngine::Velocity>(_eyeEntity);
    auto* rotation       = registry.get_component<RealEngine::Rotation>(_eyeEntity);
    auto* targetPosition = registry.get_component<RealEngine::Position>(target);

    aimAtTarget(targetPosition, registry, 2.5f, deltaTime);
    if (position && targetPosition && acceleration) {
        float dx       = targetPosition->x - position->x;
        float dy       = targetPosition->y - position->y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 10.0f) {
            velocity->vx += acceleration->ax * std::cos(rotation->angle * M_PI / 180.0f);
            velocity->vy += acceleration->ay * std::sin(rotation->angle * M_PI / 180.0f);
        }
    }
}

void EyeBomber::aimAtTarget(RealEngine::Position* targetPosition, RealEngine::Registry& registry,
                            float rotationSpeed, float deltaTime) {
    auto* position = registry.get_component<RealEngine::Position>(_eyeEntity);
    auto* rotation = registry.get_component<RealEngine::Rotation>(_eyeEntity);

    if (position && targetPosition) {
        float dx          = targetPosition->x - position->x;
        float dy          = targetPosition->y - position->y;
        float targetAngle = std::atan2(dy, dx) * 180.0f / M_PI;

        if (rotation) {
            float currentAngle    = rotation->angle;
            targetAngle           = std::fmod(targetAngle + 360.0f, 360.0f);
            currentAngle          = std::fmod(currentAngle + 360.0f, 360.0f);
            float angleDifference = targetAngle - currentAngle;

            if (angleDifference > 180.0f) {
                angleDifference -= 360.0f;
            } else if (angleDifference < -180.0f) {
                angleDifference += 360.0f;
            }
            if (angleDifference > 0.1f) {
                rotation->angle += rotationSpeed * deltaTime * 100.0f;
            } else if (angleDifference < -0.1f) {
                rotation->angle -= rotationSpeed * deltaTime * 100.0f;
            }
            rotation->angle = std::fmod(rotation->angle + 360.0f, 360.0f);
        }
    }
}

void EyeBomber::simpleBehavior(RealEngine::Registry& registry, float deltaTime) {
    auto* eyeRotation     = registry.get_component<RealEngine::Rotation>(_eyeEntity);
    auto* eyePosition     = registry.get_component<RealEngine::Position>(_eyeEntity);
    auto* eyeVelocity     = registry.get_component<RealEngine::Velocity>(_eyeEntity);
    auto* eyeAcceleration = registry.get_component<RealEngine::Acceleration>(_eyeEntity);

    eyeRotation->angle += 1.0f;
    eyeVelocity->vx += eyeAcceleration->ax * deltaTime * cos(eyeRotation->angle * M_PI / 180.0f);
    eyeVelocity->vy += eyeAcceleration->ay * deltaTime * sin(eyeRotation->angle * M_PI / 180.0f);
}

}  // namespace rtype