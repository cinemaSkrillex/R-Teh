/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Background
*/

#pragma once

#include "Engine.hpp"

namespace rtype {
class Background {
   public:
    Background(RealEngine::Registry& registry, float speed, std::string spriteStr);
    ~Background();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};
}  // namespace rtype