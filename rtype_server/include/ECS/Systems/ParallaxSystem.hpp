/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ParallaxSystem
*/

#pragma once

#include "ECS/Components/Parallax.hpp"
#include "ECS/Registry/Registry.hpp"

namespace RealEngine {
class ParallaxSystem {
   public:
    void update(Registry& registry, float deltaTime);
};
}  // namespace RealEngine