/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** TargetRadius
*/

#pragma once

#pragma once
#include <chrono>
#include <cmath>
#include <memory>

#include "../include/ECS/Components/Controllable.hpp"
#include "../include/ECS/Components/Position.hpp"
#include "../include/ECS/Components/Target.hpp"
#include "../include/ECS/Components/TargetRadius.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class TargetRadiusSystem {
   public:
    TargetRadiusSystem() {}
    ~TargetRadiusSystem() {}
    void update(Registry& registry);
};
}  // namespace RealEngine