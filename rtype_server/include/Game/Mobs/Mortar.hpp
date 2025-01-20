/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Mortar
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"

namespace rtype {
class Mortar {
   public:
    Mortar(RealEngine::Registry& registry, sf::Vector2f position);
    ~Mortar();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite                                  _mobSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _mobSpriteSheet;
};
}  // namespace rtype