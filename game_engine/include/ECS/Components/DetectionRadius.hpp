#pragma once

#include <functional>
#include "ECS/Registry/Registry.hpp"
#include "ECS/Entities/Entity.hpp"

namespace RealEngine {
struct Radius {
    float                                                                 size;
    std::function<void(RealEngine::Registry&, RealEngine::Entity, float)> behavior;
};
} // namespace RealEngine