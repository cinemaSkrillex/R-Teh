#pragma once

#include "Engine.hpp"
#include "Macros.hpp"

namespace rtype {

class EyeMinion {
   public:
    EyeMinion(RealEngine::Registry& registry, sf::Vector2f position, RealEngine::Sprite eyeSprite);
    EyeMinion(RealEngine::Registry& registry,
              sf::Vector2f          position);  // for serve rthat displays nothing
    ~EyeMinion();
    void setTarget(std::shared_ptr<RealEngine::Entity> target, RealEngine::Registry& registry);
    void agressiveBehavior(RealEngine::Registry& registry, RealEngine::Entity target,
                           float deltaTime);
    void aimAtTarget(RealEngine::Position* targetPosition, RealEngine::Registry& registry,
                     float rotationSpeed, float deltaTime);
    void simpleBehavior(RealEngine::Registry& registry, float deltaTime);
    std::shared_ptr<RealEngine::Entity> getEntity() { return _eyeEntity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _eyeEntity;
    RealEngine::Sprite                                  _eyeSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _eyeSheet;
    float                                               _directionTimer;
};
}  // namespace rtype