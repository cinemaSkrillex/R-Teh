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
    bool  killCondition = false;  // when its true kill is set to true at every destroy system loop
                                  // so reset it to false after
    bool kill = false;
};
}  // namespace RealEngine