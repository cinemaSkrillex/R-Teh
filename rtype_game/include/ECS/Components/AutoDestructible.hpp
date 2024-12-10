#pragma once

#include <functional>

namespace RealEngine {
struct AutoDestructible {
    std::function<bool()> killCondition = []() {return false;};
    float lifeTime = -1.0f;
};
}  // namespace RealEngine