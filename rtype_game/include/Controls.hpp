#pragma once

#include "ECS/Components/Acceleration.hpp"
#include "ECS/Components/Controllable.hpp"
#include "ECS/Components/Drawable.hpp"
#include "ECS/Components/Position.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/Velocity.hpp"
#include "ECS/Registry/Registry.hpp"
#include "ECS/Systems/ControlSystem.hpp"
#include "Media/Graphics/Rendering/Sprite.hpp"

namespace rtype {
class Controls {
   public:
    Controls(RealEngine::Registry& registry);
    ~Controls();
    void moveUp(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                RealEngine::Position& position, float deltaTime);
    void moveDown(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                  RealEngine::Position& position, float deltaTime);
    void moveLeft(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                  RealEngine::Position& position, float deltaTime);
    void moveRight(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                   RealEngine::Position& position, float deltaTime);
    void shoot(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
               RealEngine::Position& position, float deltaTime);

   private:
    RealEngine::Registry&            _registry;
    RealEngine::Sprite               _bulletSprite;
    std::vector<RealEngine::Entity*> _entities;
    float                            _shootCooldown;
};

}  // namespace rtype