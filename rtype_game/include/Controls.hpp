#pragma once

#include "ECS/Systems/ControlSystem.hpp"
#include "ECS/Components/Velocity.hpp"
#include "ECS/Components/Acceleration.hpp"

namespace rtype {
class Controls {
  public:
    static void moveUp(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    static void moveDown(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    static void moveLeft(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    static void moveRight(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    static void decelerateUp(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    static void decelerateDown(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    static void decelerateLeft(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    static void decelerateRight(Velocity& velocity, Acceleration& acceleration, float deltaTime);
};

} // namespace rtype