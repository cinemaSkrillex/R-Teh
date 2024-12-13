/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Netvar
*/

#pragma once

#include <any>
#include <functional>
#include <string>

#include "ECS/Registry/Registry.hpp"

#define NETVAR_UPDATE_CALLBACK_TYPE \
    std::function<void(Registry & registry, Entity & entity, float deltaTime)>

namespace RealEngine {
struct Netvar {
    std::string                 name;
    std::any                    value;
    NETVAR_UPDATE_CALLBACK_TYPE updateCallback = nullptr;
};
}  // namespace RealEngine