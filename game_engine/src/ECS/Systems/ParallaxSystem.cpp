/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ParallaxSystem
*/

#include "ECS/Systems/ParallaxSystem.hpp"

namespace RealEngine {
void ParallaxSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Parallax>();

    if (entities.empty()) {
        return;
    }

    for (auto entity : entities) {
        auto* parallax = registry.getComponent<Parallax>(entity);
        auto* position = registry.getComponent<Position>(entity);
        auto* sprite   = registry.getComponent<SpriteComponent>(entity);

        const sf::FloatRect bounds = sprite->sprite.getBounds();

        if (!parallax->initialized) {
            float scaleX = parallax->size.x / bounds.width;
            float scaleY = parallax->size.y / bounds.height;
            // Apply the scaling
            sprite->sprite.setScale(scaleX, scaleY);
            sprite->sprite.setTextureRect(0, 0, parallax->size.x * 2, parallax->size.y);
            parallax->initialized = true;
        }

        position->x += parallax->speed * deltaTime;

        if (parallax->speed < 0) {
            if (position->x <= -parallax->size.x) {
                position->x = 0;
            }
        } else {
            if (position->x >= parallax->size.x) {
                position->x = 0;
            }
        }
    }
}
}  // namespace RealEngine