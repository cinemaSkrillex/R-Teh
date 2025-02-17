/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Player
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"
#include "Game/Particles.hpp"

namespace rtype {
class PlayerEntity {
   public:
    PlayerEntity(RealEngine::Registry& registry, sf::Vector2f position);
    ~PlayerEntity();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    std::unordered_map<std::string, RealEngine::Sprite> _playerSpriteSheet;
};
}  // namespace rtype