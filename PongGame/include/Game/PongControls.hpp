/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongControls.hpp
*/

#pragma once

#include <SFML/System/Vector2.hpp>

#include "Client/UDPClient.hpp"
#include "ECS/Components/Acceleration.hpp"
#include "ECS/Components/Netvar.hpp"
#include "ECS/Components/Velocity.hpp"
#include "ECS/Registry/Registry.hpp"
#include "Shared/PongServerProtocol.hpp"

namespace pong {
class Controls {
   public:
    Controls(RealEngine::Registry& registry, std::shared_ptr<UDPClient> client);
    ~Controls();

    void moveUp(float deltaTime, RealEngine::Entity entity);
    void moveDown(float deltaTime, RealEngine::Entity entity);
    void sendReady(float deltaTime, RealEngine::Entity entity);

   private:
    RealEngine::Registry&      _registry;
    std::shared_ptr<UDPClient> _client;
};
}  // namespace pong