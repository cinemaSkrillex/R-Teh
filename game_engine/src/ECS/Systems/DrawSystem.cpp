/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** DrawSystem.cpp
*/

#include "../include/ECS/Systems/DrawSystem.hpp"

namespace RealEngine {

DrawSystem::DrawSystem(sf::RenderWindow* window) : _window(window) {}

DrawSystem::~DrawSystem() {
    if (_window) {
        delete _window;
    }
}

void calculateTextureRect(SpriteSheet& spritesheet, RealEngine::Sprite& sprite) {
    sprite.setTextureRect(spritesheet.frameSize.x * spritesheet.frameIndex, 0,
                          spritesheet.frameSize.x, spritesheet.frameSize.y);
    if (spritesheet.origin.x != -1 && spritesheet.origin.y != -1)
        sprite.setOrigin(spritesheet.origin.x, spritesheet.origin.y);
    else
        sprite.centerOrigin();
}

void handleSpriteSheetAnimation(SpriteSheet& spritesheet, RealEngine::Sprite& sprite) {
    calculateTextureRect(spritesheet, sprite);
    if (!spritesheet.pause &&
        spritesheet.animClock.getElapsedTime().asMilliseconds() > spritesheet.animTime) {
        // Check if texture is out of bounds before advancing the frame
        if (sprite.isTextureOfBounds()) {
            if (spritesheet.loop)
                spritesheet.frameIndex = 0;  // Reset frame index to 0
            else
                spritesheet.pause = true;  // Paute animation;
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

void DrawSystem::updateWithoutDisplay(Registry& registry, float deltaTime) {
    auto entities = registry.view<Position>();

    if (entities.empty()) {
        return;
    }
    for (auto entity : entities) {
        auto* position    = registry.get_component<Position>(entity);
        auto* sprite      = registry.get_component<SpriteComponent>(entity);
        auto* spritesheet = registry.get_component<SpriteSheet>(entity);

        if (sprite) {
            sprite->sprite.setPosition(position->x, position->y);
        } else if (spritesheet) {
            auto& sprite = spritesheet->sprites.at(spritesheet->spriteIndex);

            handleSpriteSheetAnimation(*spritesheet, sprite);
            sprite.setPosition(position->x, position->y);
        }
    }
}

void DrawSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Drawable, Position>();

    if (entities.empty()) {
        return;
    }
    for (auto entity : entities) {
        auto* drawable    = registry.get_component<Drawable>(entity);
        auto* position    = registry.get_component<Position>(entity);
        auto* sprite      = registry.get_component<SpriteComponent>(entity);
        auto* spritesheet = registry.get_component<SpriteSheet>(entity);

        if (sprite) {
            sprite->sprite.setPosition(position->x, position->y);
            sprite->sprite.draw(*_window);
        } else if (spritesheet) {
            auto& sprite = spritesheet->sprites.at(spritesheet->spriteIndex);

            handleSpriteSheetAnimation(*spritesheet, sprite);
            sprite.setPosition(position->x, position->y);
            sprite.draw(*_window);
        }
    }
}
}  // namespace RealEngine