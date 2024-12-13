/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** NetvarSystem
*/

#pragma once

#include "ECS/Components/Netvar.hpp"

namespace RealEngine {
class NetvarSystem {
   public:
    void update(Registry& registry, float deltaTime);
};
}  // namespace RealEngine