/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** DestroySystem
*/

#pragma once

#include "../include/ECS/Components/AutoDestructible.hpp"
#include "../include/ECS/Components/Health.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class DestroySystem {
   public:
    DestroySystem();
    ~DestroySystem() = default;
    std::vector<Entity> update(Registry& registry, float deltaTime);
};
}  // namespace RealEngine