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

enum class Action { MoveUp, MoveDown, MoveLeft, MoveRight };

class ControlSystem {
  public:
    ControlSystem() {
        // Default key bindings (can be changed with bindKey)
        keyBindings[sf::Keyboard::W] = Action::MoveUp;
        keyBindings[sf::Keyboard::S] = Action::MoveDown;
        keyBindings[sf::Keyboard::A] = Action::MoveLeft;
        keyBindings[sf::Keyboard::D] = Action::MoveRight;

        // action handlers are lambdas that take a velocity and modify it
        actionHandlers[Action::MoveUp]   = [this](Velocity& velocity) { handleMoveUp(velocity); };
        actionHandlers[Action::MoveDown] = [this](Velocity& velocity) { handleMoveDown(velocity); };
        actionHandlers[Action::MoveLeft] = [this](Velocity& velocity) { handleMoveLeft(velocity); };
        actionHandlers[Action::MoveRight] = [this](Velocity& velocity) {
            handleMoveRight(velocity);
        };
    }

    void update(Registry& registry) {
        auto& velocities    = registry.get_components<Velocity>();
        auto& controllables = registry.get_components<Controllable>();

        for (std::size_t i = 0; i < controllables.size(); ++i) {
            if (controllables[i] && velocities[i]) {
                velocities[i]->vx = 0.0f;
                velocities[i]->vy = 0.0f;

                for (const auto& [key, action] : keyBindings) {
                    if (sf::Keyboard::isKeyPressed(key)) {
                        actionHandlers[action](*velocities[i]);
                    }
                }
            }
        }
    }

    void bindKey(sf::Keyboard::Key key, Action action) { keyBindings[key] = action; }

  private:
    std::unordered_map<sf::Keyboard::Key, Action>              keyBindings;
    std::unordered_map<Action, std::function<void(Velocity&)>> actionHandlers;

    void handleMoveUp(Velocity& velocity) { velocity.vy = -10.0f; }

    void handleMoveDown(Velocity& velocity) { velocity.vy = 10.0f; }

    void handleMoveLeft(Velocity& velocity) { velocity.vx = -10.0f; }

    void handleMoveRight(Velocity& velocity) { velocity.vx = 10.0f; }
};