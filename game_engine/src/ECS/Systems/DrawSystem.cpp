/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** DrawSystem.cpp
*/

#include "../include/ECS/Systems/DrawSystem.hpp"
#include "../include/Media/Graphics/Rendering/Sprite.hpp"

DrawSystem::DrawSystem(sf::RenderWindow& window) : _window(window) {}

void calculateTextureRect(SpriteSheet& spritesheet, RealEngine::Sprite& sprite) {
    sprite.setTextureRect(spritesheet.frameSize.x * spritesheet.frameIndex, 0,
                          spritesheet.frameSize.x, spritesheet.frameSize.y);
}

void handleSpriteSheetAnimation(SpriteSheet& spritesheet, RealEngine::Sprite& sprite) {
    calculateTextureRect(spritesheet, sprite);
    if (!spritesheet.pause &&
        spritesheet.animClock.getElapsedTime().asMilliseconds() > spritesheet.animTime) {

        // Check if texture is out of bounds before advancing the frame
        if (sprite.isTextureOfBounds()) {
            if (!spritesheet.loop)
                spritesheet.frameIndex = 0; // Reset frame index to 0
            else
                spritesheet.pause = true; // Paute animation;
        } else {
            // Only increment frame if not out of bounds
            spritesheet.frameIndex++;
        }

        // Update frame index and restart clock
        spritesheet.animClock.restart();
    }

    // Update texture rect after resetting
    calculateTextureRect(spritesheet, sprite);
}

void DrawSystem::update(Registry& registry, float deltaTime, SparseArray<Position>& positions,
                        SparseArray<Drawable>& drawables, SparseArray<Sprite>& sprites,
                        SparseArray<SpriteSheet>& spritesheets) {
    for (std::size_t i = 0; i < drawables.size(); ++i) {
        if (!drawables[i] || !positions[i])
            continue;

        const auto& position = positions[i];

        if (sprites[i]) {
            auto& sprite = sprites[i]->sprite;
            sprite.setPosition(position->x, position->y);
            sprite.draw(_window);
        } else if (spritesheets[i]) {
            auto& spritesheet = spritesheets[i];
            auto& sprite      = spritesheet->sprites.at(spritesheet->spriteIndex);

            handleSpriteSheetAnimation(*spritesheet, sprite);

            sprite.setPosition(position->x, position->y);
            sprite.draw(_window);
        } else {
            // Draw fallback rectangle
            sf::RectangleShape shape(sf::Vector2f(50.0f, 50.0f));
            shape.setPosition(position->x, position->y);
            shape.setFillColor(sf::Color::Blue);
            _window.draw(shape);
        }
    }
}
