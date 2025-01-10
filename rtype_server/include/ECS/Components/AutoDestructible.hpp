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
    float lifeTime      = -1.0f;
    bool  killCondition = false;
    bool  kill          = false;
};
}  // namespace RealEngine