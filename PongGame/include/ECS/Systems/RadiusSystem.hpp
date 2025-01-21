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

   private:
};
}  // namespace RealEngine