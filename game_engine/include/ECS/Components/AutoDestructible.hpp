/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** AutoDestructible
*/

#pragma once

#include <functional>

namespace RealEngine {
struct AutoDestructible {
    float                 lifeTime      = -1.0f;
    std::function<bool()> killCondition = []() { return false; };
};
}  // namespace RealEngine