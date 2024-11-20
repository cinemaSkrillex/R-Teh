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
#include "../include/ECS/Enum/ActionEnum.hpp"

class ControlSystem {

  public:
    ControlSystem();

    void update(Registry& registry, SparseArray<Velocity>& velocities,
                SparseArray<Controllable>& controllables, SparseArray<Acceleration>& accelerations,
                float deltaTime);

    void bindKey(sf::Keyboard::Key key, Action action);

  private:
    std::unordered_map<sf::Keyboard::Key, Action>                             keyBindings;
    std::unordered_map<Action, std::function<void(Velocity&, Acceleration&)>> actionHandlers;
    std::unordered_map<Action, std::function<void(Velocity&, Acceleration&)>> actionReleaseHandlers;

    void handleMoveUp(Velocity& velocity, Acceleration& acceleration);
    void handleMoveDown(Velocity& velocity, Acceleration& acceleration);
    void handleMoveLeft(Velocity& velocity, Acceleration& acceleration);
    void handleMoveRight(Velocity& velocity, Acceleration& acceleration);

    void applyDeceleration(Velocity& velocity, Acceleration& acceleration);
    void applyUpDeceleration(Velocity& velocity, Acceleration& acceleration);
    void applyDownDeceleration(Velocity& velocity, Acceleration& acceleration);
    void applyLeftDeceleration(Velocity& velocity, Acceleration& acceleration);
    void applyRightDeceleration(Velocity& velocity, Acceleration& acceleration);
};