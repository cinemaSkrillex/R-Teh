/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBigion
*/

#pragma once

#include "Engine.hpp"

namespace rtype {

// this enemy move slowly towards the player sometimes shoot projectiles, and sometimes make eye
// bomber spawn. It have a lot of health

class EyeBigion {
   public:
    EyeBigion(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
              float speed, RealEngine::Sprite& normalMobSprite, RealEngine::Sprite& angryMobSprite);
    ~EyeBigion();
    std::shared_ptr<RealEngine::Entity> getEntity() { return _eyeEntity; }
    void setTarget(std::shared_ptr<RealEngine::Entity> target, RealEngine::Registry& registry);
    void collisionBehaviour(RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                            RealEngine::Entity collider, RealEngine::Entity entity);

   private:
    std::shared_ptr<RealEngine::Entity>                 _eyeEntity;
    RealEngine::Sprite                                  _eyeSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _eyeSheet;
};
}  // namespace rtype