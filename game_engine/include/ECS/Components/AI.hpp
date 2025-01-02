/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** AI
*/

#pragma once
#include <functional>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Registry/Registry.hpp"

namespace RealEngine {
struct AI {
    std::function<void(RealEngine::Registry&, RealEngine::Entity, float deltaTime)>
        behaviorOnTarget = [](RealEngine::Registry&, RealEngine::Entity, float) {};
    std::function<void(RealEngine::Registry&, RealEngine::Entity, float deltaTime)>
         behaviorPassive = [](RealEngine::Registry&, RealEngine::Entity, float) {};
    bool active          = false;
};
}  // namespace RealEngine
