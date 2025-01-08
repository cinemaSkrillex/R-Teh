/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Collision
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "ECS/Entities/Entity.hpp"
#include "ECS/Registry/Registry.hpp"

namespace RealEngine {

enum class CollisionType {
    INACTIVE,
    SOLID,
    PICKABLE,
    OTHER,
    PLAYER,
    ENEMY,
    ALLY_BULLET,
    ENEMY_BULLET
};

struct Collision {
    sf::FloatRect bounds;
    std::string   id;
    bool          isColliding = false;
    CollisionType type;
    std::function<void(CollisionType collisionType, Registry& registry, Entity collider,
                       Entity entity)>
        collisionActionHandler = [](CollisionType, Registry&, Entity, Entity) {};
    // sf::Vector2f size; for later.
};
}  // namespace RealEngine