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
        auto* currentCollision = registry.get_component<Collision>(other);

        if (entity == other) {
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
        auto* collision   = registry.get_component<Collision>(entity);
        auto* sprite      = registry.get_component<SpriteComponent>(entity);
        auto* spritesheet = registry.get_component<SpriteSheet>(entity);

        if (sprite) {
            collision->bounds = sprite->sprite.getBounds();
        }
        if (spritesheet) {
            collision->bounds = spritesheet->sprites.at(spritesheet->spriteIndex).getBounds();
        }
    }
    for (auto entity : entities) {
        auto* collision = registry.get_component<Collision>(entity);
        isCollidingWithOthers(*collision, registry, entity);
    }
}
}  // namespace RealEngine