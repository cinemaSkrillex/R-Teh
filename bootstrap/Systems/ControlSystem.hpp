/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** InputSystem.hpp
*/

#pragma once
#include <SFML/Window.hpp>
#include "Registry.hpp"
#include "Velocity.hpp"
#include "Controllable.hpp"
#include "Acceleration.hpp"

enum class Action { MoveUp, MoveDown, MoveLeft, MoveRight };

class ControlSystem {
  public:
    ControlSystem() {
        // Default key bindings (can be changed with bindKey)
        keyBindings[sf::Keyboard::Z] = Action::MoveUp;
        keyBindings[sf::Keyboard::S] = Action::MoveDown;
        keyBindings[sf::Keyboard::Q] = Action::MoveLeft;
        keyBindings[sf::Keyboard::D] = Action::MoveRight;

        // Initialize action handlers
        actionHandlers[Action::MoveUp] = [this](Velocity& velocity, Acceleration& acceleration) {
            handleMoveUp(velocity, acceleration);
        };
        actionHandlers[Action::MoveDown] = [this](Velocity& velocity, Acceleration& acceleration) {
            handleMoveDown(velocity, acceleration);
        };
        actionHandlers[Action::MoveLeft] = [this](Velocity& velocity, Acceleration& acceleration) {
            handleMoveLeft(velocity, acceleration);
        };
        actionHandlers[Action::MoveRight] = [this](Velocity& velocity, Acceleration& acceleration) {
            handleMoveRight(velocity, acceleration);
        };
    }

    void update(Registry& registry, SparseArray<Velocity>& velocities,
                SparseArray<Controllable>& controllables, SparseArray<Acceleration>& accelerations,
                float deltaTime) {
        for (std::size_t i = 0; i < controllables.size(); ++i) {
            if (controllables[i] && velocities[i] && accelerations[i]) {
                bool keyPressed = false;
                for (const auto& [key, action] : keyBindings) {
                    if (sf::Keyboard::isKeyPressed(key)) {
                        keyPressed = true;
                        actionHandlers[action](*velocities[i], *accelerations[i]);
                    }
                }
                if (!keyPressed && accelerations[i]->air_friction == true) {
                    applyDeceleration(*velocities[i], *accelerations[i]);
                } else if (!keyPressed && accelerations[i]->air_friction == false) {
                    velocities[i]->vx = 0;
                    velocities[i]->vy = 0;
                }
            }
        }
    }

    void bindKey(sf::Keyboard::Key key, Action action) { keyBindings[key] = action; }

  private:
    std::unordered_map<sf::Keyboard::Key, Action>                             keyBindings;
    std::unordered_map<Action, std::function<void(Velocity&, Acceleration&)>> actionHandlers;

    void handleMoveUp(Velocity& velocity, Acceleration& acceleration) {
        velocity.vy -= acceleration.ay;
    }

    void handleMoveDown(Velocity& velocity, Acceleration& acceleration) {
        velocity.vy += acceleration.ay;
    }

    void handleMoveLeft(Velocity& velocity, Acceleration& acceleration) {
        velocity.vx -= acceleration.ax;
    }

    void handleMoveRight(Velocity& velocity, Acceleration& acceleration) {
        velocity.vx += acceleration.ax;
    }

    void applyDeceleration(Velocity& velocity, Acceleration& acceleration) {
        if (velocity.vx > 0) {
            velocity.vx -= acceleration.ax;
            if (velocity.vx < 0)
                velocity.vx = 0;
        } else if (velocity.vx < 0) {
            velocity.vx += acceleration.ax;
            if (velocity.vx > 0)
                velocity.vx = 0;
        }

        if (velocity.vy > 0) {
            velocity.vy -= acceleration.ay;
            if (velocity.vy < 0)
                velocity.vy = 0;
        } else if (velocity.vy < 0) {
            velocity.vy += acceleration.ay;
            if (velocity.vy > 0)
                velocity.vy = 0;
        }
    }
};