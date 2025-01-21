/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** CollisionSystem
*/

#include "../include/ECS/Systems/CollisionSystem.hpp"

namespace RealEngine {

CollisionSystem::CollisionSystem() {}

void isCollidingWithOthers(std::optional<Collision> collision, Registry& registry, Entity entity) {
    auto entities = registry.view<Collision>();

    for (auto other : entities) {
        auto* currentCollision = registry.getComponent<Collision>(other);

        if ((entity == other) || (currentCollision->id.compare(collision->id) == 0)) {
            continue;
        }
        if (collision->bounds.intersects(currentCollision->bounds)) {
            if (collision->collisionActionHandler) {
                collision->collisionActionHandler(currentCollision->type, registry, other, entity);
            }
        }
    }
}

void CollisionSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Collision>();

    if (entities.empty()) {
        return;
    }
    for (auto entity : entities) {
        auto* collision   = registry.getComponent<Collision>(entity);
        auto* sprite      = registry.getComponent<SpriteComponent>(entity);
        auto* spritesheet = registry.getComponent<SpriteSheet>(entity);

        if (collision->type == CollisionType::SCREEN) {
            continue;
        }
        if (sprite) {
            collision->bounds = sprite->sprite.getBounds();
        }
        if (spritesheet) {
            collision->bounds = spritesheet->sprites.at(spritesheet->spriteIndex).getBounds();
        }
    }
    for (auto entity : entities) {
        auto* collision = registry.getComponent<Collision>(entity);
        isCollidingWithOthers(*collision, registry, entity);
    }
}
}  // namespace RealEngine