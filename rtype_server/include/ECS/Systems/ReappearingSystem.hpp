/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ReappearingSystem.hpp
*/

#pragma once
#include <limits>

#include "../include/ECS/Components/Position.hpp"
#include "../include/ECS/Components/Reappearing.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class ReappearingSystem {
   public:
    void update(Registry& registry, float deltaTime);  // Assurez-vous que la signature correspond
};
}  // namespace RealEngine
