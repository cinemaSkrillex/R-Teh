#pragma once
#include <SFML/Graphics.hpp>

#include "../include/ECS/Components/Collision.hpp"
#include "../include/ECS/Components/SpriteSheet.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class RadiusSystem {
   public:
    RadiusSystem();
    ~RadiusSystem();

    void update(Registry& registry, float deltaTime);

   private:
};
}  // namespace RealEngine