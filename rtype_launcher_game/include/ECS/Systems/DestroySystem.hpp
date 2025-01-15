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
    void                killEntity(Registry& registry, Entity entity);
    bool                autoDestroy(Registry& registry, Entity entity, float deltaTime);
    bool                healthDestroy(Registry& registry, Entity entity);
    void                update(Registry& registry, float deltaTime);
    std::vector<Entity> getDeadEntities() { return _deadEntities; }

   private:
    std::vector<Entity> _deadEntities;
};
}  // namespace RealEngine