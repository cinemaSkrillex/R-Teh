#pragma once

#include <SFML/System/Vector2.hpp>

#include "Client/UDPClient.hpp"
#include "ECS/Components/Acceleration.hpp"
#include "ECS/Components/AutoDestructible.hpp"
#include "ECS/Components/Controllable.hpp"
#include "ECS/Components/Drawable.hpp"
#include "ECS/Components/Position.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/Velocity.hpp"
#include "ECS/Registry/Registry.hpp"
#include "ECS/Systems/ControlSystem.hpp"
#include "Macros.hpp"
#include "Media/Graphics/Rendering/Sprite.hpp"

namespace rtype {
class Controls {
   public:
    Controls(RealEngine::Registry& registry, std::shared_ptr<UDPClient> client);
    ~Controls();
    void moveUp(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                RealEngine::Position& position, float deltaTime);
    void moveDown(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                  RealEngine::Position& position, float deltaTime);
    void moveLeft(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                  RealEngine::Position& position, float deltaTime);
    void moveRight(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                   RealEngine::Position& position, float deltaTime);
    void shoot(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
               RealEngine::Position& position, float deltaTime);
    void holdShoot(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                   RealEngine::Position& position, float deltaTime);
    void releaseShoot(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                      RealEngine::Position& position, float deltaTime);

   private:
    RealEngine::Registry&      _registry;
    std::shared_ptr<UDPClient> _client;
};
}  // namespace rtype