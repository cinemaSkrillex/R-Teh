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
    actionReleaseHandlers[Action::Action2] = [](Velocity&, Acceleration&, Position&, float) {};
    actionReleaseHandlers[Action::Action3] = [](Velocity&, Acceleration&, Position&, float) {};
    actionReleaseHandlers[Action::Action4] = [](Velocity&, Acceleration&, Position&, float) {};

    actionHoldHandlers[Action::Up]      = [](Velocity&, Acceleration&, Position&, float) {};
    actionHoldHandlers[Action::Down]    = [](Velocity&, Acceleration&, Position&, float) {};
    actionHoldHandlers[Action::Left]    = [](Velocity&, Acceleration&, Position&, float) {};
    actionHoldHandlers[Action::Right]   = [](Velocity&, Acceleration&, Position&, float) {};
    actionHoldHandlers[Action::Action1] = [](Velocity&, Acceleration&, Position&, float) {};
    actionHoldHandlers[Action::Action2] = [](Velocity&, Acceleration&, Position&, float) {};
    actionHoldHandlers[Action::Action3] = [](Velocity&, Acceleration&, Position&, float) {};
    actionHoldHandlers[Action::Action4] = [](Velocity&, Acceleration&, Position&, float) {};

    _holdTreashold = 0.3f;
}

void ControlSystem::update(Registry& registry, float deltaTime) {
    if (!_window.isFocused()) return;

    auto entities = registry.view<Velocity, Acceleration, Position, Controllable>();
    if (entities.empty()) return;

    for (auto entity : entities) {
        auto* velocity     = registry.get_component<Velocity>(entity);
        auto* acceleration = registry.get_component<Acceleration>(entity);
        auto* position     = registry.get_component<Position>(entity);

        for (const auto& [key, action] : keyBindings) {
            bool  isPressed = sf::Keyboard::isKeyPressed(key);
            auto& state     = keyStates[key];

            if (isPressed) {
                if (!state.supported) {
                    actionHandlers[action](*velocity, *acceleration, *position, deltaTime);
                    continue;
                }
                if (!state.isHeld) {
                    // Key press start
                    state.isHeld   = true;
                    state.holdTime = 0.0f;
                } else {
                    // Key still held
                    state.holdTime += deltaTime;
                }
                // call hold handler if hold time is greater than 0.1f
                if (state.holdTime > _holdTreashold) {
                    actionHoldHandlers[action](*velocity, *acceleration, *position, deltaTime);
                }
            } else {
                if (state.isHeld) {
                    // Key released
                    state.isHeld = false;
                    if (state.holdTime < _holdTreashold) {
                        // Short press
                        actionHandlers[action](*velocity, *acceleration, *position, deltaTime);
                    } else {
                        // Long press
                        actionReleaseHandlers[action](*velocity, *acceleration, *position,
                                                      deltaTime);
                    }
                }
            }
        }
    }
}

void ControlSystem::bindKey(sf::Keyboard::Key key, Action action, bool supportHold) {
    keyBindings[key]         = action;
    keyStates[key].supported = supportHold;
}

void ControlSystem::setActionHandler(Action action, ActionHandler handler) {
    actionHandlers[action] = handler;
}

void ControlSystem::setActionReleaseHandler(Action action, ActionHandler handler) {
    actionReleaseHandlers[action] = handler;
}

void ControlSystem::setActionHoldHandler(Action action, ActionHandler handler) {
    actionHoldHandlers[action] = handler;
}

sf::Keyboard::Key ControlSystem::getBoundKey(Action action) {
    for (const auto& [key, act] : keyBindings) {
        if (act == action) return key;
    }
    return sf::Keyboard::Unknown;
}

bool ControlSystem::isActionPressed(Action action) {
    for (const auto& [key, act] : keyBindings) {
        if (act == action && sf::Keyboard::isKeyPressed(key)) return true;
    }
    return false;
}
}  // namespace RealEngine