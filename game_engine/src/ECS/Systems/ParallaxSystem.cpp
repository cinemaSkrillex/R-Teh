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
}
}  // namespace RealEngine