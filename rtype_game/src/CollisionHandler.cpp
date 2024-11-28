#include "CollisionHandler.hpp"

void handle_collision(RealEngine::Registry&                                      registry,
                      const std::unordered_map<std::string, RealEngine::Entity>& entities) {
    auto& collisions = registry.get_components<RealEngine::Collision>();
    auto& positions  = registry.get_components<RealEngine::Position>();

    RealEngine::Entity spaceship2 = entities.at("spaceship2");
    RealEngine::Entity sol        = entities.at("ground");

    auto& spaceship2_collision = collisions[spaceship2];
    auto& spaceship2_position  = positions[spaceship2];
    auto& sol_collision        = collisions[sol];
    auto& sol_position         = positions[sol];

    if (spaceship2_collision && sol_collision) {
        if (spaceship2_collision->bounds.intersects(sol_collision->bounds)) {
            if (spaceship2_position->y + spaceship2_collision->bounds.height > sol_position->y) {
                spaceship2_position->y = sol_position->y - spaceship2_collision->bounds.height;
            }
        }
    }
}