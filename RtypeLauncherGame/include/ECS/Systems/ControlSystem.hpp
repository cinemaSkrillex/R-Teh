/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** InputSystem.hpp
*/

#pragma once
#include <SFML/Window.hpp>
#include <unordered_map>

#include "../include/Core/Window.hpp"
#include "../include/ECS/Components/Acceleration.hpp"
#include "../include/ECS/Components/Controllable.hpp"
#include "../include/ECS/Components/Position.hpp"
#include "../include/ECS/Components/Velocity.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {

// Define a struct to track the state of keys
struct KeyState {
    bool  supported = false;
    bool  isHeld    = false;
    float holdTime  = 0.0f;  // How long the key has been held
};

class ControlSystem {
   public:
    ControlSystem(Window& window);

    using ActionHandler = std::function<void(float, RealEngine::Entity)>;
    void              update(Registry& registry, float deltaTime);
    void              bindKey(sf::Keyboard::Key key, Action action, bool supportHold = false);
    void              setActionHandler(Action action, ActionHandler handler);
    void              setActionReleaseHandler(Action action, ActionHandler handler);
    void              setActionHoldHandler(Action action, ActionHandler handler);
    sf::Keyboard::Key getBoundKey(Action action);
    bool              isActionPressed(Action action);
    float             getHoldTreashold() const { return _holdTreashold; }
    void              setHoldTreashold(float treashold) { _holdTreashold = treashold; }

   private:
    std::unordered_map<sf::Keyboard::Key, Action>   _keyBindings;
    std::unordered_map<Action, ActionHandler>       _actionHandlers;
    std::unordered_map<Action, ActionHandler>       _actionReleaseHandlers;
    std::unordered_map<Action, ActionHandler>       _actionHoldHandlers;
    Window&                                         _window;
    std::unordered_map<sf::Keyboard::Key, KeyState> _keyStates;
    float                                           _holdTreashold;
};
}  // namespace RealEngine