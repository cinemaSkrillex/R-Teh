#pragma once

#include <RtypeServerProtocol.hpp>
#include <SFML/System/Vector2.hpp>

#include "Client/UDPClient.hpp"
#include "Engine.hpp"
#include "Particles.hpp"

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
    float                      _particlesTimer;
};
}  // namespace rtype