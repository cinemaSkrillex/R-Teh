/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeMinion
*/

#pragma once

#include "Engine.hpp"

namespace rtype {

// this enemy have some boosts towards the player and shoot projectiles

class Block {
   public:
    Block(RealEngine::Registry& registry, sf::Vector2f position, const std::string& spriteName,
          float rotation);
    ~Block();
    std::shared_ptr<RealEngine::Entity> getEntity() { return _blockEntity; }

   private:
    std::shared_ptr<RealEngine::Entity> _blockEntity;
    RealEngine::Sprite                  _blockSprite;
};
}  // namespace rtype