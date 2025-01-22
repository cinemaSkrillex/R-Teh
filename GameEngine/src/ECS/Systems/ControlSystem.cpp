/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ControlSystem.cpp
*/

#include "../include/ECS/Systems/ControlSystem.hpp"

namespace RealEngine {

ControlSystem::ControlSystem(Window& window) : _window(window) {
    _actionHandlers[Action::Up]      = [](float, RealEngine::Entity) {};
    _actionHandlers[Action::Down]    = [](float, RealEngine::Entity) {};
    _actionHandlers[Action::Left]    = [](float, RealEngine::Entity) {};
    _actionHandlers[Action::Right]   = [](float, RealEngine::Entity) {};
    _actionHandlers[Action::Action1] = [](float, RealEngine::Entity) {};
    _actionHandlers[Action::Action2] = [](float, RealEngine::Entity) {};
    _actionHandlers[Action::Action3] = [](float, RealEngine::Entity) {};
    _actionHandlers[Action::Action4] = [](float, RealEngine::Entity) {};

    _actionReleaseHandlers[Action::Up]      = [](float, RealEngine::Entity) {};
    _actionReleaseHandlers[Action::Down]    = [](float, RealEngine::Entity) {};
    _actionReleaseHandlers[Action::Left]    = [](float, RealEngine::Entity) {};
    _actionReleaseHandlers[Action::Right]   = [](float, RealEngine::Entity) {};
    _actionReleaseHandlers[Action::Action1] = [](float, RealEngine::Entity) {};
    _actionReleaseHandlers[Action::Action2] = [](float, RealEngine::Entity) {};
    _actionReleaseHandlers[Action::Action3] = [](float, RealEngine::Entity) {};
    _actionReleaseHandlers[Action::Action4] = [](float, RealEngine::Entity) {};

    _actionHoldHandlers[Action::Up]      = [](float, RealEngine::Entity) {};
    _actionHoldHandlers[Action::Down]    = [](float, RealEngine::Entity) {};
    _actionHoldHandlers[Action::Left]    = [](float, RealEngine::Entity) {};
    _actionHoldHandlers[Action::Right]   = [](float, RealEngine::Entity) {};
    _actionHoldHandlers[Action::Action1] = [](float, RealEngine::Entity) {};
    _actionHoldHandlers[Action::Action2] = [](float, RealEngine::Entity) {};
    _actionHoldHandlers[Action::Action3] = [](float, RealEngine::Entity) {};
    _actionHoldHandlers[Action::Action4] = [](float, RealEngine::Entity) {};

    _holdTreashold = 0.3f;
}

void ControlSystem::update(Registry& registry, float deltaTime) {
    if (!_window.isFocused()) return;

    auto entities = registry.view<Velocity, Acceleration, Position,
                                  Controllable>();  // WARNING Controllable is sufficient
    if (entities.empty()) return;

    for (auto entity : entities) {
        for (const auto& [key, action] : _keyBindings) {
            bool  isPressed = sf::Keyboard::isKeyPressed(key);
            auto& state     = _keyStates[key];

            if (isPressed) {
                if (!state.supported) {
                    _actionHandlers[action](deltaTime, entity);
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
                    _actionHoldHandlers[action](deltaTime, entity);
                }
            } else {
                if (state.isHeld) {
                    // Key released
                    state.isHeld = false;
                    if (state.holdTime < _holdTreashold) {
                        // Short press
                        _actionHandlers[action](deltaTime, entity);
                    } else {
                        // Long press
                        _actionReleaseHandlers[action](deltaTime, entity);
                    }
                }
            }
        }
    }
}

void ControlSystem::bindKey(sf::Keyboard::Key key, Action action, bool supportHold) {
    _keyBindings[key]         = action;
    _keyStates[key].supported = supportHold;
}

void ControlSystem::setActionHandler(Action action, ActionHandler handler) {
    _actionHandlers[action] = handler;
}

void ControlSystem::setActionReleaseHandler(Action action, ActionHandler handler) {
    _actionReleaseHandlers[action] = handler;
}

void ControlSystem::setActionHoldHandler(Action action, ActionHandler handler) {
    _actionHoldHandlers[action] = handler;
}

sf::Keyboard::Key ControlSystem::getBoundKey(Action action) {
    for (const auto& [key, act] : _keyBindings) {
        if (act == action) return key;
    }
    return sf::Keyboard::Unknown;
}

bool ControlSystem::isActionPressed(Action action) {
    for (const auto& [key, act] : _keyBindings) {
        if (act == action && sf::Keyboard::isKeyPressed(key)) return true;
    }
    return false;
}
}  // namespace RealEngine