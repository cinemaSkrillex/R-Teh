/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** DrawSystem.cpp
*/

#include "../include/ECS/Systems/DrawSystem.hpp"

DrawSystem::DrawSystem(sf::RenderWindow& window) : _window(window) {}

void DrawSystem::update(Registry& registry, float deltaTime, SparseArray<Position>& positions,
                        SparseArray<Drawable>& drawables, SparseArray<Sprite>& sprites) {
    for (std::size_t i = 0; i < drawables.size(); ++i) {
        if (!drawables[i])
            continue;

        if (!positions[i])
            continue;

        if (sprites[i]) {
            sf::Texture texture;
            sf::Sprite sprite;

            // load texture
            if (!texture.loadFromFile(sprites[i]->filePath)) {
                std::cerr << "Drawable: Failed to load texture: " << sprites[i]->filePath << std::endl;
                texture.loadFromFile("../assets/missing_texture.png");
            }

            // load sprite
            sprite.setTexture(texture);
            sprite.setPosition(positions[i]->x, positions[i]->y);

            // render sprite
            _window.draw(sprite);
        } else {
            sf::RectangleShape shape(sf::Vector2f(50.0f, 50.0f));
            shape.setPosition(positions[i]->x, positions[i]->y);
            shape.setFillColor(sf::Color::Blue);
            _window.draw(shape);
        }
    }
}