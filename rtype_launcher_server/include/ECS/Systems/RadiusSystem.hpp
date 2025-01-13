#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

#include "../include/ECS/Components/Controllable.hpp"
#include "../include/ECS/Components/Position.hpp"
#include "../include/ECS/Components/Radius.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class RadiusSystem {
   public:
    RadiusSystem();
    ~RadiusSystem();

    void update(Registry& registry);
    void setBehaviorIn(Entity entity, Registry& registry, std::function<void()> behavior);
    void setBehaviorOuterMax(Entity entity, Registry& registry, std::function<void()> behavior);
    void setBehaviorOuterMin(Entity entity, Registry& registry, std::function<void()> behavior);

   private:
};
}  // namespace RealEngine