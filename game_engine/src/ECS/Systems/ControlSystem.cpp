/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ControlSystem.cpp
*/

#include "../include/ECS/Systems/ControlSystem.hpp"

ControlSystem::ControlSystem() {
    keyBindings[sf::Keyboard::Z] = Action::Up;
    keyBindings[sf::Keyboard::S] = Action::Down;
    keyBindings[sf::Keyboard::Q] = Action::Left;
    keyBindings[sf::Keyboard::D] = Action::Right;
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

void ControlSystem::setActionHandler(Action action, ActionHandler handler) {
    actionHandlers[action] = handler;
}

void ControlSystem::setActionReleaseHandler(Action action, ActionHandler handler) {
    actionReleaseHandlers[action] = handler;
}
