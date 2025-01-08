#pragma once
#include "../include/ECS/Components/AI.hpp"
#include "../include/ECS/Components/Collision.hpp"
#include "../include/ECS/Components/SpriteSheet.hpp"
#include "../include/ECS/Components/Target.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class AISystem {
   public:
    AISystem();
    ~AISystem();

    void update(Registry& registry, float deltaTime);
    void attachPassiveBehavior(Registry& registry, Entity entity,
                               std::function<void(Registry&, Entity, float)> behavior);
    void attachTargetBehavior(Registry& registry, Entity entity,
                              std::function<void(Registry&, Entity, float)> behavior);
    void setActive(Registry& registry, Entity entity, bool active);

   private:
};
}  // namespace RealEngine