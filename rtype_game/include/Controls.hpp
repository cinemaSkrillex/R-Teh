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
    Controls(Registry& registry);
    ~Controls();
    void moveUp(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void moveDown(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void moveLeft(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void moveRight(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void decelerateUp(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void decelerateDown(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void decelerateLeft(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void decelerateRight(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void shoot(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void voidAction(Velocity& velocity, Acceleration& acceleration, float deltaTime);

  private:
    Registry&                        _registry;
    std::vector<Entity*>             _entities;
    std::vector<RealEngine::Sprite*> _sprites;
};

} // namespace rtype