#include "Game.hpp"

namespace rtype {

void Game::player_collision_handler(RealEngine::CollisionType collisionType,
                                      RealEngine::Registry& registry, RealEngine::Entity collider, RealEngine::Entity entity) {
    std::cout << "Player collided" << std::endl;
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            player_collide_with_ground();
            break;
        // case RealEngine::CollisionType::HIT:
        //     player_take_damage(registry, collider);
        //     break;
        case RealEngine::CollisionType::PICKABLE:
            //handle pickable
            break;
        case RealEngine::CollisionType::OTHER:
            break;
        case RealEngine::CollisionType::ENEMY:
            player_take_damage(collider);
            break;
        case RealEngine::CollisionType::ALLY_BULLET:
            break;
        case RealEngine::CollisionType::ENEMY_BULLET:
            player_take_damage(collider);
            break;
        default:
            break;
    }
}

void Game::player_collide_with_ground() {
    auto* playerPosition = _registry.get_component<RealEngine::Position>(_entity2);
    auto* playerVelocity = _registry.get_component<RealEngine::Velocity>(_entity2);

    playerVelocity->vy = 0;
    playerVelocity->vx = 0;
    playerPosition->y -= 1;
    playerPosition->x -= 1;
}

void Game::player_take_damage(RealEngine::Entity collider) {
    auto* playerHealth   = _registry.get_component<RealEngine::Health>(_entity2);
    auto* colliderDamage = _registry.get_component<RealEngine::Damage>(collider);

    if (playerHealth) {
        if (colliderDamage) {
            playerHealth->damage += colliderDamage->amount;
            std::cout << "Player took " << colliderDamage->amount << " damage" << std::endl;
        }
    }
}
}  // namespace rtype