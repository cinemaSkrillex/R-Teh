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

namespace RealEngine {

#define NETVAR_UPDATE_CALLBACK_TYPE \
    std::function<void(Registry & registry, Entity & entity, float deltaTime)>

struct Netvar {
    std::string                 type;
    std::string                 name;
    std::any                    value;
    NETVAR_UPDATE_CALLBACK_TYPE updateCallback = nullptr;
};
}  // namespace RealEngine