#pragma once

#include <functional>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Registry/Registry.hpp"

namespace RealEngine {

enum class RadiusState { NONE = 0, IN = 1, OUTER_MAX = 2, OUTER_MIN = 3 };

struct Radius {
    float                   sizeMax;
    float                   sizeMin;
    std::function<void()>   behaviorIn       = []() {};
    std::function<void()>   behaviorOuterMax = []() {};
    std::function<void()>   behaviorOuterMin = []() {};
    RealEngine::RadiusState lastState        = RealEngine::RadiusState::NONE;
};
}  // namespace RealEngine