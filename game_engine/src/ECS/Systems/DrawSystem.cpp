/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** DrawSystem.cpp
*/

#include "DrawSystem.hpp"

DrawSystem::DrawSystem(sf::RenderWindow& window) : _window(window) {}

void DrawSystem::update(Registry& registry, SparseArray<Position>& positions,
                        SparseArray<Drawable>& drawables, float deltaTime) {
    for (std::size_t i = 0; i < drawables.size(); ++i) {
        if (drawables[i] && positions[i]) {
            sf::RectangleShape shape(sf::Vector2f(50.0f, 50.0f));
            shape.setPosition(positions[i]->x, positions[i]->y);
            shape.setFillColor(sf::Color::Green);
            _window.draw(shape);
        }
    }
}