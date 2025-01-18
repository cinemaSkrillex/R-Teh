/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MobSpawnerShip
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"
#include "Game/Mobs/SpacePlane.hpp"

namespace rtype {
class MobSpawnerShip {
   public:
    MobSpawnerShip(RealEngine::Registry& registry, sf::Vector2f position);
    ~MobSpawnerShip();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite                                  _mobSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _mobSpriteSheet;
};
}  // namespace rtype