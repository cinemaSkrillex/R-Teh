#include "../include/ECS/Systems/RotationSystem.hpp"

#include <iostream>
namespace RealEngine {

RotationSystem::RotationSystem() {}

RotationSystem::~RotationSystem() {}

void RotationSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Rotation, Drawable>();

    if (entities.empty()) {
        return;
    }
    for (auto entity : entities) {
        auto* rotation    = registry.getComponent<Rotation>(entity);
        auto* sprite      = registry.getComponent<SpriteComponent>(entity);
        auto* spritesheet = registry.getComponent<SpriteSheet>(entity);

        if (rotation && sprite) {
            sprite->sprite.setRotation(rotation->angle);
        } else if (rotation && spritesheet) {
            auto& sprite = spritesheet->sprites.at(spritesheet->spriteIndex);
            sprite.setRotation(rotation->angle);
        }
    }
}

}  // namespace RealEngine
