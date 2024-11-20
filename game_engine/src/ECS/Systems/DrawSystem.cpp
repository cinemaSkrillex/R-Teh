/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** DrawSystem.cpp
*/

#include "../include/ECS/Systems/DrawSystem.hpp"
#include "../include/Media/Graphics/Rendering/Sprite.hpp"

DrawSystem::DrawSystem(sf::RenderWindow& window) : _window(window) {}

void DrawSystem::update(Registry& registry, float deltaTime, SparseArray<Position>& positions,
                        SparseArray<Drawable>& drawables, SparseArray<Sprite>& sprites,
                        SparseArray<SpriteSheet>& spritesheets) {
    for (std::size_t i = 0; i < drawables.size(); ++i) {
        if (!drawables[i])
            continue;

        if (!positions[i])
            continue;

        if (sprites[i]) {
            sprites[i]->sprite.setPosition(positions[i]->x, positions[i]->y);
            // render sprite
            sprites[i]->sprite.draw(_window);
        } else if (spritesheets[i]) {
            std::cout << "spritesheet detected" << std::endl;
        } else {
            sf::RectangleShape shape(sf::Vector2f(50.0f, 50.0f));
            shape.setPosition(positions[i]->x, positions[i]->y);
            shape.setFillColor(sf::Color::Blue);
            _window.draw(shape);
        }
    }
}