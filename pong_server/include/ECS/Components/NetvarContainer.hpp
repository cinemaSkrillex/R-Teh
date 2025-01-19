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

#include "ECS/Components/Netvar.hpp"
#include "ECS/Registry/Registry.hpp"

namespace RealEngine {
struct NetvarContainer {
    std::unordered_map<std::string, Netvar> netvars;

    void addNetvar(const std::string& name, const Netvar& netvar) { netvars[name] = netvar; }

    Netvar* getNetvar(const std::string& name) {
        auto it = netvars.find(name);
        if (it != netvars.end()) return &it->second;
        return nullptr;
    }

    void removeNetvar(const std::string& name) { netvars.erase(name); }
};

}  // namespace RealEngine