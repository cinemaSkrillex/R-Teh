#pragma once
#include "../include/ECS/Components/Collision.hpp"
#include "../include/ECS/Components/SpriteSheet.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class AISystem {
   public:
    AISystem();
    ~AISystem();

    void update(Registry& registry, float deltaTime);
    void attachBehavior(Registry& registry, float deltaTime);

   private:
};
}  // namespace RealEngine