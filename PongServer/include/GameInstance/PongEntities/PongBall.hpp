/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongBall.hpp
*/

#pragma once

#include <SFML/System/Vector2.hpp>

#include "Core/AssetsManager.hpp"
#include "ECS/Components/Acceleration.hpp"
#include "ECS/Components/Collision.hpp"
#include "ECS/Components/Drawable.hpp"
#include "ECS/Components/Interpolation.hpp"
#include "ECS/Components/NetvarContainer.hpp"
#include "ECS/Components/Position.hpp"
#include "ECS/Components/Score.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/Velocity.hpp"
#include "ECS/Registry/Registry.hpp"

namespace pong {
class PongBall {
   public:
    PongBall(RealEngine::Registry& registry, sf::Vector2f position);
    ~PongBall();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};
}  // namespace pong