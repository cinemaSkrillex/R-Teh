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
    std::unordered_map<sf::Keyboard::Key, Action>                                    keyBindings;
    std::unordered_map<Action, std::function<void(Velocity&, Acceleration&, float)>> actionHandlers;
    std::unordered_map<Action, std::function<void(Velocity&, Acceleration&, float)>>
        actionReleaseHandlers;

    void handleMoveUp(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void handleMoveDown(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void handleMoveLeft(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void handleMoveRight(Velocity& velocity, Acceleration& acceleration, float deltaTime);

    void applyDeceleration(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void applyUpDeceleration(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void applyDownDeceleration(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void applyLeftDeceleration(Velocity& velocity, Acceleration& acceleration, float deltaTime);
    void applyRightDeceleration(Velocity& velocity, Acceleration& acceleration, float deltaTime);
};