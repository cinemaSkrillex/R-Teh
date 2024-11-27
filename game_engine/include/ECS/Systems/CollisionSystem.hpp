/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** CollisionSystem
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "../include/ECS/Registry/Registry.hpp"
#include "../include/ECS/Components/Collision.hpp"
#include "../include/ECS/Components/SpriteSheet.hpp"

namespace RealEngine {
class CollisionSystem {

  public:
    CollisionSystem();

    void update(Registry& registry, SparseArray<Collision>& collisions,
                SparseArray<SpriteComponent>& sprites, SparseArray<SpriteSheet>& spritesheets,
                float deltaTime);

  private:
};
} // namespace RealEngine