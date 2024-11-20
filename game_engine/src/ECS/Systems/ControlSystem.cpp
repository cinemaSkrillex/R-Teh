/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ControlSystem.cpp
*/

#include "../include/ECS/Systems/ControlSystem.hpp"

ControlSystem::ControlSystem() {
    // Default key bindings (can be changed with bindKey)
    keyBindings[sf::Keyboard::Z] = Action::Up;
    keyBindings[sf::Keyboard::S] = Action::Down;
    keyBindings[sf::Keyboard::Q] = Action::Left;
    keyBindings[sf::Keyboard::D] = Action::Right;

    // Initialize action handlers
    actionHandlers[Action::Up] = [this](Velocity& velocity, Acceleration& acceleration,
                                        float deltaTime) {
        handleMoveUp(velocity, acceleration, deltaTime);
    };
    actionHandlers[Action::Down] = [this](Velocity& velocity, Acceleration& acceleration,
                                          float deltaTime) {
        handleMoveDown(velocity, acceleration, deltaTime);
    };
    actionHandlers[Action::Left] = [this](Velocity& velocity, Acceleration& acceleration,
                                          float deltaTime) {
        handleMoveLeft(velocity, acceleration, deltaTime);
    };
    actionHandlers[Action::Right] = [this](Velocity& velocity, Acceleration& acceleration,
                                           float deltaTime) {
        handleMoveRight(velocity, acceleration, deltaTime);
    };

    actionReleaseHandlers[Action::Up] = [this](Velocity& velocity, Acceleration& acceleration,
                                               float deltaTime) {
        applyUpDeceleration(velocity, acceleration, deltaTime);
    };
    actionReleaseHandlers[Action::Down] = [this](Velocity& velocity, Acceleration& acceleration,
                                                 float deltaTime) {
        applyDownDeceleration(velocity, acceleration, deltaTime);
    };
    actionReleaseHandlers[Action::Left] = [this](Velocity& velocity, Acceleration& acceleration,
                                                 float deltaTime) {
        applyLeftDeceleration(velocity, acceleration, deltaTime);
    };
    actionReleaseHandlers[Action::Right] = [this](Velocity& velocity, Acceleration& acceleration,
                                                  float deltaTime) {
        applyRightDeceleration(velocity, acceleration, deltaTime);
    };
}

void ControlSystem::update(Registry& registry, SparseArray<Velocity>& velocities,
                           SparseArray<Controllable>& controllables,
                           SparseArray<Acceleration>& accelerations, float deltaTime) {
    for (std::size_t i = 0; i < controllables.size(); ++i) {
        if (!controllables[i])
            continue;

        if (!velocities[i]) {
            std::cerr << "ControlSystem: Velocity Component missing on entity(" << i << ")."
                      << std::endl;
            continue;
        }

        if (!accelerations[i]) {
            std::cerr << "ControlSystem: Acceleration Component missing on entity(" << i << ")."
                      << std::endl;
        }

        bool keyPressed = false;
        for (const auto& [key, Action] : keyBindings) {
            if (sf::Keyboard::isKeyPressed(key)) {
                actionHandlers[Action](*velocities[i], *accelerations[i], deltaTime * 100.0);
            } else {
                if (accelerations[i]->air_friction == true) {
                    actionReleaseHandlers[Action](*velocities[i], *accelerations[i],
                                                  deltaTime * 100.0);
                } else {
                    velocities[i]->vx = 0;
                    velocities[i]->vy = 0;
                }
            }
        }
    }
}

void ControlSystem::bindKey(sf::Keyboard::Key key, Action action) { keyBindings[key] = action; }

void ControlSystem::handleMoveUp(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vy > 50)
        velocity.vy = 50;
    velocity.vy -= acceleration.ay * 3 * deltaTime;
    if (velocity.vy < -400)
        velocity.vy = -400;
}

void ControlSystem::handleMoveDown(Velocity& velocity, Acceleration& acceleration,
                                   float deltaTime) {
    if (velocity.vy < -50)
        velocity.vy = -50;
    velocity.vy += acceleration.ay * 3 * deltaTime;
    if (velocity.vy > 400)
        velocity.vy = 400;
}

void ControlSystem::handleMoveLeft(Velocity& velocity, Acceleration& acceleration,
                                   float deltaTime) {
    if (velocity.vx > 50)
        velocity.vx = 50;
    velocity.vx -= acceleration.ax * 3 * deltaTime;
    if (velocity.vx < -400)
        velocity.vx = -400;
}

void ControlSystem::handleMoveRight(Velocity& velocity, Acceleration& acceleration,
                                    float deltaTime) {
    if (velocity.vx < -50)
        velocity.vx = -50;
    velocity.vx += acceleration.ax * 3 * deltaTime;
    if (velocity.vx > 400)
        velocity.vx = 400;
}

void ControlSystem::applyUpDeceleration(Velocity& velocity, Acceleration& acceleration,
                                        float deltaTime) {
    if (velocity.vy < 0) {
        velocity.vy += acceleration.ay * 3 * deltaTime;
        if (velocity.vy > 0)
            velocity.vy = 0;
    }
}

void ControlSystem::applyDownDeceleration(Velocity& velocity, Acceleration& acceleration,
                                          float deltaTime) {
    if (velocity.vy > 0) {
        velocity.vy -= acceleration.ay * 3 * deltaTime;
        if (velocity.vy < 0)
            velocity.vy = 0;
    }
}

void ControlSystem::applyLeftDeceleration(Velocity& velocity, Acceleration& acceleration,
                                          float deltaTime) {
    if (velocity.vx < 0) {
        velocity.vx += acceleration.ax * 3 * deltaTime;
        if (velocity.vx > 0)
            velocity.vx = 0;
    }
}

void ControlSystem::applyRightDeceleration(Velocity& velocity, Acceleration& acceleration,
                                           float deltaTime) {
    if (velocity.vx > 0) {
        velocity.vx -= acceleration.ax * 3 * deltaTime;
        if (velocity.vx < 0)
            velocity.vx = 0;
    }
}
