#pragma once

#include <functional>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Registry/Registry.hpp"

namespace RealEngine {

enum class RadiusState { NONE, IN, OUTER_MAX, OUTER_MIN };

struct Radius {
    float                 sizeMax;
    float                 sizeMin;
    std::function<void()> behaviorIn       = []() {};
    std::function<void()> behaviorOuterMax = []() {};
    std::function<void()> behaviorOuterMin = []() {};
    RadiusState           lastState        = RadiusState::NONE;
};
}  // namespace RealEngine