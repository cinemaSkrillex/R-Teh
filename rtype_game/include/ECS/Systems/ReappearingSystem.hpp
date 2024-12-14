/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ReappearingSystem.hpp
*/

#pragma once
#include "../include/ECS/Components/Position.hpp"
#include "../include/ECS/Components/Reappearing.hpp"
#include "../include/ECS/Registry/Registry.hpp"
#include <limits>

namespace RealEngine {
class ReappearingSystem {
   public:
    void update(Registry& registry, float deltaTime); // Assurez-vous que la signature correspond
};
}  // namespace RealEngine
