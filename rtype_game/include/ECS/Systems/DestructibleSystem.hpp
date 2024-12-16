#pragma once

#include "../include/ECS/Components/AutoDestructible.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class DestructibleSystem {
   public:
    DestructibleSystem();
    ~DestructibleSystem() = default;
    std::vector<Entity> update(Registry& registry, float deltaTime);
};
}  // namespace RealEngine