#pragma once
#include <functional>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Registry/Registry.hpp"

namespace RealEngine {
struct AI {
    std::function<void(RealEngine::Registry&, RealEngine::Entity, float deltaTime)>
        behaviorOnTarget = [](RealEngine::Registry&, RealEngine::Entity, float) {};
    std::function<void(RealEngine::Registry&, float deltaTime)> behaviorPassive =
        [](RealEngine::Registry&, float) {};
    bool active = false;
};
}  // namespace RealEngine