/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** lagComprensation
*/

#pragma once

#include "../include/ECS/Components/Interpolation.hpp"
#include "../include/ECS/Components/Position.hpp"
#include "../include/ECS/Registry/Registry.hpp"
#include "../include/Interpolation.hpp"

namespace RealEngine {
class LagCompensationSystem {
   public:
    LagCompensationSystem();
    ~LagCompensationSystem();

    void update(Registry &registry, float deltaTime);
};
}  // namespace RealEngine