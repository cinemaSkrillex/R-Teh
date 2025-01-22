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

namespace RealEngine {

class LagCompensationSystem {
   public:
    LagCompensationSystem();
    ~LagCompensationSystem();

    void update(Registry& registry, float deltaTime);
};

template <typename T>
T lerp(const T& a, const T& b, float t);

template <typename T>
T invlerp(const T& a, const T& b, const T& v);

}  // namespace RealEngine