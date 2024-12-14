/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PlayerEntity
*/

#pragma once

#include "Engine.hpp"

namespace rtype {
class Player {
   public:
    Player(RealEngine::Registry& registry, sf::Vector2f position,
           std::unordered_map<std::string, RealEngine::Sprite> playerSprites);
    ~Player();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _playerEntity; }
    void player_collision_handler(RealEngine::CollisionType collisionType,
                                  RealEngine::Registry& registry, RealEngine::Entity collider);
    void player_collide_with_ground(RealEngine::Registry& registry);
    void player_take_damage(RealEngine::Registry& registry, RealEngine::Entity collider);

   private:
    std::shared_ptr<RealEngine::Entity>                 _playerEntity;
    std::unordered_map<std::string, RealEngine::Sprite> _spaceshipSheet;
    RealEngine::Sprite                                  _idlePlayerSprite;
    RealEngine::Sprite                                  _upPlayerSprite;
    RealEngine::Sprite                                  _downPlayerSprite;
};
}  // namespace rtype