#pragma once
#include <functional>
#include "ECS/Registry/Registry.hpp"
#include "ECS/Entities/Entity.hpp"

namespace RealEngine {
struct AI {
    bool                                                                  active;
    bool                                                                  enemy;
    std::function<void(RealEngine::Registry&, RealEngine::Entity, float)> behavior;
};
} // namespace RealEngine