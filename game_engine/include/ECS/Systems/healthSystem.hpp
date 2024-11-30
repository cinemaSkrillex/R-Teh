#pragma once

#include "../include/ECS/Components/Health.hpp"

namespace RealEngine {
class HealthSystem {
   public:
    void update(Registry& registry, float deltaTime);
};
}  // namespace RealEngine