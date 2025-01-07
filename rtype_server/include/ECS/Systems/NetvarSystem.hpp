/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** NetvarSystem
*/

#pragma once

#include "ECS/Components/Netvar.hpp"
#include "ECS/Components/NetvarContainer.hpp"

namespace RealEngine {
class NetvarSystem {
   public:
    void update(Registry& registry, float deltaTime);
    void updateNetvars(Registry& registry, float deltaTime);
    void updateNetvarsContainers(Registry& registry, float deltaTime);
};
}  // namespace RealEngine