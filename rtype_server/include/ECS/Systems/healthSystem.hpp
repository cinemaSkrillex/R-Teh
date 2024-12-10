#pragma once

#include "../include/ECS/Components/Health.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class HealthSystem {
   public:
    void update(Registry& registry, float deltaTime);
};
}  // namespace RealEngine