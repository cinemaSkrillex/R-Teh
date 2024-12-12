/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** CollisionSystem
*/

#pragma once
#include <SFML/Graphics.hpp>

#include "../include/ECS/Components/Collision.hpp"
#include "../include/ECS/Components/SpriteSheet.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class CollisionSystem {
   public:
    CollisionSystem();
    ~CollisionSystem() = default;

    void update(Registry& registry, float deltaTime);

   private:
};
}  // namespace RealEngine