/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeMinion
*/

#pragma once

#include "BaseBlock.hpp"

namespace rtype {

class Block : public BaseBlock {
   public:
    Block(RealEngine::Registry& registry, sf::Vector2f position, const std::string& spriteName,
          float rotation, float scrollingSpeed, RealEngine::CollisionType collisionType);
    ~Block();

    void initialize(RealEngine::Registry& registry, sf::Vector2f position,
                    const std::string& spriteName, float rotation, float scrollingSpeed,
                    RealEngine::CollisionType collisionType) override;
    std::shared_ptr<RealEngine::Entity> getEntity() override { return _blockEntity; }
    const std::string&                  getElement() const override { return _element; }

   private:
    std::shared_ptr<RealEngine::Entity> _blockEntity;
    RealEngine::Sprite                  _blockSprite;
    std::string                         _element;
};

}  // namespace rtype