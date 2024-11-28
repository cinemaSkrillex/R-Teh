/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** InputSystem.hpp
*/

#pragma once
#include <SFML/Window.hpp>
#include "../include/ECS/Registry/Registry.hpp"
#include "../include/ECS/Components/Velocity.hpp"
#include "../include/ECS/Components/Controllable.hpp"
#include "../include/ECS/Components/Acceleration.hpp"
#include "../include/ECS/Components/Position.hpp"
#include "../include/ECS/Enum/ActionEnum.hpp"

namespace RealEngine {
class ControlSystem {

  public:
    ControlSystem();

    using ActionHandler = std::function<void(Velocity&, Acceleration&, Position&, float)>;
    void update(Registry& registry, float deltaTime);
    void bindKey(sf::Keyboard::Key key, Action action);
    void setActionHandler(Action action, ActionHandler handler);
    void setActionReleaseHandler(Action action, ActionHandler handler);

  private:
    std::unordered_map<sf::Keyboard::Key, Action> keyBindings;
    std::unordered_map<Action, ActionHandler>     actionHandlers;
    std::unordered_map<Action, ActionHandler>     actionReleaseHandlers;
};
} // namespace RealEngine