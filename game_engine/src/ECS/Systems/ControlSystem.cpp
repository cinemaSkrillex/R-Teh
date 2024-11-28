/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ControlSystem.cpp
*/

#include "../include/ECS/Systems/ControlSystem.hpp"

namespace RealEngine {

ControlSystem::ControlSystem() {
    keyBindings[sf::Keyboard::Z] = Action::Up;
    keyBindings[sf::Keyboard::S] = Action::Down;
    keyBindings[sf::Keyboard::Q] = Action::Left;
    keyBindings[sf::Keyboard::D] = Action::Right;
}

void ControlSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Velocity, Acceleration, Position, Controllable>();

    for (auto entity : entities) {
        auto* velocity     = registry.get_component<Velocity>(entity);
        auto* acceleration = registry.get_component<Acceleration>(entity);
        auto* position     = registry.get_component<Position>(entity);

        for (const auto& [key, action] : keyBindings) {
            if (sf::Keyboard::isKeyPressed(key)) {
                actionHandlers[action](*velocity, *acceleration, *position, deltaTime * 100.0);
            } else {
                if (acceleration->air_friction) {
                    actionReleaseHandlers[action](*velocity, *acceleration, *position,
                                                  deltaTime * 100.0);
                } else {
                    velocity->vx = 0;
                    velocity->vy = 0;
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

} // namespace RealEngine