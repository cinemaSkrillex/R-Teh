/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** DrawSystem.cpp
*/

#include "../include/ECS/Systems/DrawSystem.hpp"

namespace RealEngine {

DrawSystem::DrawSystem(sf::RenderTexture* window) : _window(window) {}

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
        if (sprite.isTextureOfBounds()) {
            if (spritesheet.loop)
                spritesheet.frameIndex = 0;
            else
                spritesheet.pause = true;
        } else {
            spritesheet.frameIndex++;
        }
        spritesheet.animClock.restart();
    }
    calculateTextureRect(spritesheet, sprite);
}

void DrawSystem::updateParticles(Registry& registry, float deltaTime) {
    auto particleEmitters = registry.view<ParticleEmitter>();

    for (auto entity : particleEmitters) {
        std::cout << "Drawing particles" << std::endl;
        auto* emitter = registry.get_component<ParticleEmitter>(entity);
        for (const auto& particle : emitter->particles) {
            sf::CircleShape shape(particle.size);
            shape.setPosition(particle.position);
            shape.setFillColor(particle.color);
            _window->draw(shape);
        }
    }
}

std::vector<std::pair<RealEngine::Sprite, int>> DrawSystem::updateWithoutDisplay(Registry& registry,
                                                                                 float deltaTime) {
    std::vector<std::pair<RealEngine::Sprite, int>> spritesWithZIndex;
    auto                                            entities = registry.view<Position>();

    if (entities.empty()) {
        return spritesWithZIndex;
    }
    for (auto entity : entities) {
        auto* position    = registry.get_component<Position>(entity);
        auto* sprite      = registry.get_component<SpriteComponent>(entity);
        auto* spritesheet = registry.get_component<SpriteSheet>(entity);

        if (sprite) {
            sprite->sprite.setPosition(position->x, position->y);
            spritesWithZIndex.emplace_back(sprite->sprite, sprite->zIndex);
        } else if (spritesheet) {
            auto& sprite = spritesheet->sprites.at(spritesheet->spriteIndex);

            handleSpriteSheetAnimation(*spritesheet, sprite);
            sprite.setPosition(position->x, position->y);
            spritesWithZIndex.emplace_back(sprite, spritesheet->zIndex);
        }
    }
    // std::cout << "Entites" << entities.size() << std::endl;
    // std::cout << "Drawing " << spritesWithZIndex.size() << " sprites" << std::endl;
    return spritesWithZIndex;
}

void DrawSystem::update(Registry& registry, float deltaTime) {
    auto spritesWithZIndex = updateWithoutDisplay(registry, deltaTime);
    std::sort(spritesWithZIndex.begin(), spritesWithZIndex.end(),
              [](const std::pair<RealEngine::Sprite, int>& a,
                 const std::pair<RealEngine::Sprite, int>& b) { return a.second < b.second; });
    for (auto& pair : spritesWithZIndex) {
        // std::cout << "Drawing sprite with z-index: " << pair.second << std::endl;
        pair.first.draw(*_window);
    }
    // std::cout << "Drawing particles" << std::endl;
    updateParticles(registry, deltaTime);
}
}  // namespace RealEngine