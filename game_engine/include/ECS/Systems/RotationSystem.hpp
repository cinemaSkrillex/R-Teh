#pragma once
#include "../include/ECS/Components/Drawable.hpp"
#include "../include/ECS/Components/Rotation.hpp"
#include "../include/ECS/Components/Sprite.hpp"
#include "../include/ECS/Components/SpriteSheet.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class RotationSystem {
   public:
    RotationSystem();
    ~RotationSystem();

    void update(Registry& registry, float deltaTime);
    void setRotation(Registry& registry, float angle);
    void rotate(Registry& registry, float angle);
};
}  // namespace RealEngine