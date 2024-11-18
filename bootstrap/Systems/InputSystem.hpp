/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** InputSystem.hpp
*/

#pragma once
#include <SFML/Window.hpp>
#include "Registry.hpp"
#include "Velocity.hpp"
#include "Controllable.hpp"

class InputSystem {
  public:
    void update(Registry& registry) {
        auto& velocities    = registry.get_components<Velocity>();
        auto& controllables = registry.get_components<Controllable>();

        for (std::size_t i = 0; i < controllables.size(); ++i) {
            if (controllables[i] && velocities[i]) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    velocities[i]->vy = -10.0f;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    velocities[i]->vy = 10.0f;
                } else {
                    velocities[i]->vy = 0.0f;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    velocities[i]->vx = -10.0f;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    velocities[i]->vx = 10.0f;
                } else {
                    velocities[i]->vx = 0.0f;
                }
            }
        }
    }
};