#pragma once

#include "../include/ECS/Components/AutoDestructible.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class DestructibleSystem {
   public:
    DestructibleSystem();
    ~DestructibleSystem() = default;
    void update(Registry& registry, float deltaTime);
};
}  // namespace RealEngine