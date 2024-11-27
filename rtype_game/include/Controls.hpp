#pragma once

#include "ECS/Systems/ControlSystem.hpp"
#include "ECS/Components/Velocity.hpp"
#include "ECS/Components/Acceleration.hpp"
#include "ECS/Registry/Registry.hpp"
#include "ECS/Components/Position.hpp"
#include "ECS/Components/Drawable.hpp"
#include "ECS/Components/Controllable.hpp"
#include "ECS/Components/Sprite.hpp"
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
    void decelerateUp(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                      RealEngine::Position& position, float deltaTime);
    void decelerateDown(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                        RealEngine::Position& position, float deltaTime);
    void decelerateLeft(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                        RealEngine::Position& position, float deltaTime);
    void decelerateRight(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                         RealEngine::Position& position, float deltaTime);
    void shoot(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
               RealEngine::Position& position, float deltaTime);
    void voidAction(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                    RealEngine::Position& position, float deltaTime);

  private:
    RealEngine::Registry&            _registry;
    std::vector<RealEngine::Entity*> _entities;
    std::vector<RealEngine::Sprite*> _sprites;
};

} // namespace rtype