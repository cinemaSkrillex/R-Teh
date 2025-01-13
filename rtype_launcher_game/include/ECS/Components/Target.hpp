#pragma once

#include "ECS/Entities/Entity.hpp"

namespace RealEngine {
struct Target {
    std::shared_ptr<Entity> target;
};
}  // namespace RealEngine