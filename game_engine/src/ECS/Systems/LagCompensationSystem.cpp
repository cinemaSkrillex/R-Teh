/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** LagCompensation
*/

#include "ECS/Systems/LagCompensationSystem.hpp"

namespace RealEngine {
LagCompensationSystem::LagCompensationSystem() {}

LagCompensationSystem::~LagCompensationSystem() {}

void LagCompensationSystem::update(Registry &registry, float deltaTime) {
    auto entities = registry.view<Interpolation, Position>();

    if (entities.empty()) return;
    for (auto entity : entities) {
        Interpolation *interpolation = registry.getComponent<Interpolation>(entity);
        Position      *position      = registry.getComponent<Position>(entity);
        if (interpolation->reset) {
            position->x          = interpolation->end.x;
            position->y          = interpolation->end.y;
            interpolation->reset = false;
        }
        sf::Vector2f newPos = LagCompensation::lerp(interpolation->start, interpolation->end,
                                                    interpolation->currentStep);
        position->x         = newPos.x;
        position->y         = newPos.y;

        interpolation->currentStep =
            std::min(interpolation->currentStep + interpolation->step * deltaTime, 1.0f);
    }
}
}  // namespace RealEngine