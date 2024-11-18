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

class RenderSystem {
  public:
    RenderSystem(sf::RenderWindow& window) : _window(window) {}

    void update(Registry& registry) {
        auto& positions = registry.get_components<Position>();
        auto& drawables = registry.get_components<Drawable>();

        for (std::size_t i = 0; i < drawables.size(); ++i) {
            if (drawables[i] && positions[i]) {
                sf::RectangleShape shape(sf::Vector2f(50.0f, 50.0f));
                shape.setPosition(positions[i]->x, positions[i]->y);
                shape.setFillColor(sf::Color::Green);
                _window.draw(shape);
            }
        }
    }

  private:
    sf::RenderWindow& _window;
};