/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RenderSystem.hpp
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "Registry.hpp"
#include "Position.hpp"
#include "Drawable.hpp"
#include "Sprite.hpp"

class DrawSystem {
  public:
    DrawSystem(sf::RenderWindow& window);
    void update(Registry& registry, float deltaTime, SparseArray<Position>& positions,
                SparseArray<Drawable>& drawables, SparseArray<Sprite>& sprites);

  private:
    sf::RenderWindow& _window;
};