/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Player
*/

#pragma once

#include "Mobs/Behaviors.hpp"

namespace rtype {
class Player {
   public:
    Player(RealEngine::Registry& registry, sf::Vector2f position, bool otherPlayer);
    ~Player();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    std::unordered_map<std::string, RealEngine::Sprite> _playerSpriteSheet;
};
}  // namespace rtype