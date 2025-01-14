/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** TargetRadius
*/

#pragma once

#include "ECS/Entities/Entity.hpp"
#include "ECS/Registry/Registry.hpp"

namespace RealEngine {

struct TargetRadius {
    float size;
    bool  focusOnTarget = true;
};
}  // namespace RealEngine