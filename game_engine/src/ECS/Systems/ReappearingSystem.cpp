/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ReappearingSystem.cpp
*/


#include "../include/ECS/Systems/ReappearingSystem.hpp"

namespace RealEngine {

// void ReappearingSystem::update(Registry& registry, float deltaTime) {
//     auto entities = registry.view<Position, Reappearing>();

//     for (auto entity : entities) {
//         auto* position = registry.get_component<Position>(entity);
//         auto* reappearing = registry.get_component<Reappearing>(entity);

//         std::cout << "Entity Position X: " << position->x << ", minX: " << reappearing->minX << std::endl;
//         if (position && reappearing) {
//             std::cout << "Entity Position X: " << position->x << ", minX: " << reappearing->minX << std::endl;
//             if (position->x < reappearing->minX) {
//                 std::cout << "Reappearing! New X: " << reappearing->reappearX << std::endl;
//                 position->x = reappearing->reappearX;
//             }
//         }
//     }
// }


void ReappearingSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Position, Reappearing>();

    // Collecter les positions actuelles pour gérer les positions relatives
    std::vector<std::pair<Entity, Position*>> entityPositions;

    for (auto entity : entities) {
        auto* position = registry.get_component<Position>(entity);
        auto* reappearing = registry.get_component<Reappearing>(entity);
        if (position && reappearing) {
            entityPositions.push_back({entity, position});
        }
    }

    // Vérifie chaque entité pour gérer la réapparition
    for (auto& [entity, position] : entityPositions) {
        auto* reappearing = registry.get_component<Reappearing>(entity);

        // Vérifie si l'entité a quitté l'écran par la gauche
        if (position->x + reappearing->width < reappearing->minX) {
            // Trouver la position maximale pour enchaîner dynamiquement
            float maxX = -std::numeric_limits<float>::infinity();
            for (auto& [otherEntity, otherPosition] : entityPositions) {
                if (otherEntity != entity) {
                    maxX = std::max(maxX, otherPosition->x);
                }
            }

            // Réapparaît à la suite de l'autre
            position->x = maxX + reappearing->width;
        }
    }
}


}  // namespace RealEngine
