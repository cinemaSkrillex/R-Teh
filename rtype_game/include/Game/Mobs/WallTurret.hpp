/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** WallTurret
*/

#pragma once

#include "Engine.hpp"

namespace rtype {
class WallTurret {
   public:
    WallTurret(RealEngine::Registry& registry, sf::Vector2f position, bool onGround);
    ~WallTurret();
    std::shared_ptr<RealEngine::Entity> getEntity() { return _canonEntity; }
    void setTarget(std::shared_ptr<RealEngine::Entity> target, RealEngine::Registry& registry);
    void collisionBehaviour(RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                            RealEngine::Entity collider, RealEngine::Entity entity);

   private:
    std::shared_ptr<RealEngine::Entity> _canonEntity;
    std::shared_ptr<RealEngine::Entity> _pedestalEntity;
    RealEngine::Sprite                  _canonSprite;
    RealEngine::Sprite                  _pedestalSprite;
};
}  // namespace rtype