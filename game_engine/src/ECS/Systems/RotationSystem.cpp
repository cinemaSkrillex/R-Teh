#include "../include/ECS/Systems/RotationSystem.hpp"

#include <iostream>
namespace RealEngine {

RotationSystem::RotationSystem() {}

RotationSystem::~RotationSystem() {}

void RotationSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Rotation, Drawable>();

    for (auto entity : entities) {
        auto* rotation    = registry.get_component<Rotation>(entity);
        auto* sprite      = registry.get_component<SpriteComponent>(entity);
        auto* spritesheet = registry.get_component<SpriteSheet>(entity);

        if (rotation && sprite) {
            sprite->sprite.setRotation(rotation->angle);
        } else if (rotation && spritesheet) {
            auto& sprite = spritesheet->sprites.at(spritesheet->spriteIndex);
            sprite.setRotation(rotation->angle);
        }
    }
}

void RotationSystem::setRotation(Registry& registry, float angle) {
    auto entities = registry.view<Rotation>();

    for (auto entity : entities) {
        auto* rotation = registry.get_component<Rotation>(entity);

        if (rotation) {
            rotation->angle = angle;
        }
    }
}

void RotationSystem::rotate(Registry& registry, float angle) {
    auto entities = registry.view<Rotation>();

    for (auto entity : entities) {
        auto* rotation = registry.get_component<Rotation>(entity);

        if (rotation) {
            rotation->angle += angle;
        }
    }
}

}  // namespace RealEngine