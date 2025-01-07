/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ControlSystem.cpp
*/

#include "../include/ECS/Systems/ControlSystem.hpp"

namespace RealEngine {

ControlSystem::ControlSystem(Window& window) : _window(window) {
    actionHandlers[Action::Up]      = [](float, RealEngine::Entity) {};
    actionHandlers[Action::Down]    = [](float, RealEngine::Entity) {};
    actionHandlers[Action::Left]    = [](float, RealEngine::Entity) {};
    actionHandlers[Action::Right]   = [](float, RealEngine::Entity) {};
    actionHandlers[Action::Action1] = [](float, RealEngine::Entity) {};
    actionHandlers[Action::Action2] = [](float, RealEngine::Entity) {};
    actionHandlers[Action::Action3] = [](float, RealEngine::Entity) {};
    actionHandlers[Action::Action4] = [](float, RealEngine::Entity) {};

    actionReleaseHandlers[Action::Up]      = [](float, RealEngine::Entity) {};
    actionReleaseHandlers[Action::Down]    = [](float, RealEngine::Entity) {};
    actionReleaseHandlers[Action::Left]    = [](float, RealEngine::Entity) {};
    actionReleaseHandlers[Action::Right]   = [](float, RealEngine::Entity) {};
    actionReleaseHandlers[Action::Action1] = [](float, RealEngine::Entity) {};
    actionReleaseHandlers[Action::Action2] = [](float, RealEngine::Entity) {};
    actionReleaseHandlers[Action::Action3] = [](float, RealEngine::Entity) {};
    actionReleaseHandlers[Action::Action4] = [](float, RealEngine::Entity) {};

    actionHoldHandlers[Action::Up]      = [](float, RealEngine::Entity) {};
    actionHoldHandlers[Action::Down]    = [](float, RealEngine::Entity) {};
    actionHoldHandlers[Action::Left]    = [](float, RealEngine::Entity) {};
    actionHoldHandlers[Action::Right]   = [](float, RealEngine::Entity) {};
    actionHoldHandlers[Action::Action1] = [](float, RealEngine::Entity) {};
    actionHoldHandlers[Action::Action2] = [](float, RealEngine::Entity) {};
    actionHoldHandlers[Action::Action3] = [](float, RealEngine::Entity) {};
    actionHoldHandlers[Action::Action4] = [](float, RealEngine::Entity) {};

    _holdTreashold = 0.3f;
}

void ControlSystem::update(Registry& registry, float deltaTime) {
    if (!_window.isFocused()) return;

    auto entities = registry.view<Velocity, Acceleration, Position, Controllable>();
    if (entities.empty()) return;

    for (auto entity : entities) {
        for (const auto& [key, action] : keyBindings) {
            bool  isPressed = sf::Keyboard::isKeyPressed(key);
            auto& state     = keyStates[key];

            if (isPressed) {
                if (!state.supported) {
                    actionHandlers[action](deltaTime, entity);
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
                    actionHoldHandlers[action](deltaTime, entity);
                }
            } else {
                if (state.isHeld) {
                    // Key released
                    state.isHeld = false;
                    if (state.holdTime < _holdTreashold) {
                        // Short press
                        actionHandlers[action](deltaTime, entity);
                    } else {
                        // Long press
                        actionReleaseHandlers[action](deltaTime, entity);
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