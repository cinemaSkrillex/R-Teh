/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Controls
*/

#pragma once

#include <RtypeServerProtocol.hpp>

#include "Client/UDPClient.hpp"
#include "Engine.hpp"

namespace rtype {
class Controls {
   public:
    Controls(RealEngine::Registry& registry, std::shared_ptr<UDPClient> client);
    ~Controls();

    void moveUp(float deltaTime, RealEngine::Entity entity);
    void moveDown(float deltaTime, RealEngine::Entity entity);
    void moveLeft(float deltaTime, RealEngine::Entity entity);
    void moveRight(float deltaTime, RealEngine::Entity entity);
    void shoot(float deltaTime, RealEngine::Entity entity);
    void holdShoot(float deltaTime, RealEngine::Entity entity);
    void releaseShoot(float deltaTime, RealEngine::Entity entity);

   private:
    RealEngine::Registry&      _registry;
    std::shared_ptr<UDPClient> _client;
};
}  // namespace rtype