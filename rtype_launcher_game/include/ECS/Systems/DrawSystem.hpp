/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RenderSystem.hpp
*/

#pragma once
#include <SFML/Graphics.hpp>

#include "../include/Core/Graphics/Rendering/Sprite.hpp"
#include "../include/ECS/Components/Drawable.hpp"
#include "../include/ECS/Components/ParticleEmitter.hpp"
#include "../include/ECS/Components/Position.hpp"
#include "../include/ECS/Components/SpriteSheet.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class DrawSystem {
   public:
    DrawSystem(sf::RenderWindow* window = nullptr);
    ~DrawSystem();
    std::vector<std::pair<RealEngine::Sprite, int>> updateWithoutDisplay(Registry& registry,
                                                                         float     deltaTime);
    void                                            update(Registry& registry, float deltaTime);

   private:
    sf::RenderWindow* _window;

    void updateParticles(Registry& registry, float deltaTime);
};
}  // namespace RealEngine