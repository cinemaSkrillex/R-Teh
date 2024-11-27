/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RenderSystem.hpp
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "../include/ECS/Registry/Registry.hpp"
#include "../include/ECS/Components/Position.hpp"
#include "../include/ECS/Components/Drawable.hpp"
#include "../include/ECS/Components/SpriteSheet.hpp"

namespace RealEngine {
class DrawSystem {
  public:
    DrawSystem(sf::RenderWindow& window);
    void update(Registry& registry, float deltaTime, SparseArray<Position>& positions,
                SparseArray<Drawable>& drawables, SparseArray<SpriteComponent>& sprites,
                SparseArray<SpriteSheet>& spritesheets);

  private:
    sf::RenderWindow& _window;
};
} // namespace RealEngine