/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ControlSystem.cpp
*/

#include "../include/ECS/Systems/ControlSystem.hpp"

namespace RealEngine {

ControlSystem::ControlSystem(Window& window) : _window(window) {
    actionHandlers[Action::Up]      = [](Velocity&, Acceleration&, Position&, float) {};
    actionHandlers[Action::Down]    = [](Velocity&, Acceleration&, Position&, float) {};
    actionHandlers[Action::Left]    = [](Velocity&, Acceleration&, Position&, float) {};
    actionHandlers[Action::Right]   = [](Velocity&, Acceleration&, Position&, float) {};
    actionHandlers[Action::Action1] = [](Velocity&, Acceleration&, Position&, float) {};
    actionHandlers[Action::Action2] = [](Velocity&, Acceleration&, Position&, float) {};
    actionHandlers[Action::Action3] = [](Velocity&, Acceleration&, Position&, float) {};
    actionHandlers[Action::Action4] = [](Velocity&, Acceleration&, Position&, float) {};

    actionReleaseHandlers[Action::Up]      = [](Velocity&, Acceleration&, Position&, float) {};
    actionReleaseHandlers[Action::Down]    = [](Velocity&, Acceleration&, Position&, float) {};
    actionReleaseHandlers[Action::Left]    = [](Velocity&, Acceleration&, Position&, float) {};
    actionReleaseHandlers[Action::Right]   = [](Velocity&, Acceleration&, Position&, float) {};
    actionReleaseHandlers[Action::Action1] = [](Velocity&, Acceleration&, Position&, float) {};
    actionHandlers[Action::Action2]        = [](Velocity&, Acceleration&, Position&, float) {};
    actionHandlers[Action::Action3]        = [](Velocity&, Acceleration&, Position&, float) {};
    actionHandlers[Action::Action4]        = [](Velocity&, Acceleration&, Position&, float) {};
}

void ControlSystem::update(Registry& registry, float deltaTime) {
    if (!_window.isFocused()) return;
    auto entities = registry.view<Velocity, Acceleration, Position, Controllable>();

    if (entities.empty()) {
        return;
    }
    for (auto entity : entities) {
        auto* velocity     = registry.get_component<Velocity>(entity);
        auto* acceleration = registry.get_component<Acceleration>(entity);
        auto* position     = registry.get_component<Position>(entity);

        for (const auto& [key, action] : keyBindings) {
            if (sf::Keyboard::isKeyPressed(key)) {
                actionHandlers[action](*velocity, *acceleration, *position, deltaTime * 100.0);
            } else {
                actionReleaseHandlers[action](*velocity, *acceleration, *position,
                                              deltaTime * 100.0);
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

sf::Keyboard::Key ControlSystem::getBoundKey(Action action) {
    for (const auto& [key, act] : keyBindings) {
        if (act == action) return key;
    }
    return sf::Keyboard::Unknown;
}

bool ControlSystem::isActionPressed(Action action) {
    for (const auto& [key, act] : keyBindings) {
        if (act == action && sf::Keyboard::isKeyPressed(key))
            return true;
    }
    return false;
}
}  // namespace RealEngine