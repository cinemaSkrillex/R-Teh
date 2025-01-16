/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Radius
*/

#pragma once

#include <functional>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Registry/Registry.hpp"

namespace RealEngine {

enum class RadiusState { ABSENT = 0, INSIDE = 1, OUTER_MAX = 2, OUTER_MIN = 3 };

struct Radius {
    float                                                          sizeMax;
    float                                                          sizeMin;
    std::function<void(RealEngine::Registry&, RealEngine::Entity)> behaviorIn =
        [](RealEngine::Registry&, RealEngine::Entity) {};
    std::function<void(RealEngine::Registry&, RealEngine::Entity)> behaviorOuterMax =
        [](RealEngine::Registry&, RealEngine::Entity) {};
    std::function<void(RealEngine::Registry&, RealEngine::Entity)> behaviorOuterMin =
        [](RealEngine::Registry&, RealEngine::Entity) {};
    RealEngine::RadiusState lastState = RealEngine::RadiusState::ABSENT;
};
}  // namespace RealEngine