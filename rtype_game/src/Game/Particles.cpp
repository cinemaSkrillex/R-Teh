/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Particles
*/

#include "Game/Particles.hpp"

namespace rtype {

ParticlesClass::ParticlesClass(sf::Vector2f position, ParticleType type) {
    switch (type) {
        case EXPLOSION:
            _particle = createExplosion(position);
            break;
        case HIT_EFFECT:
            _particle = createHitEffect(position);
            break;
        case SHOOT_LOAD:
            _particle = createShootLoad(position);
            break;
        default:
            break;
    }
}

ParticlesClass::~ParticlesClass() {}

RealEngine::ParticleEmitter ParticlesClass::createExplosion(sf::Vector2f position) {
    return RealEngine::ParticleEmitter{{},
                                       {position.x, position.y},
                                       {10, 0},
                                       2.0f,
                                       5.0f,
                                       sf::Color::White,
                                       sf::Color::Transparent,
                                       2.0f,
                                       0.0f};
}

RealEngine::ParticleEmitter ParticlesClass::createHitEffect(sf::Vector2f position) {
    return RealEngine::ParticleEmitter{{},
                                       {position.x, position.y},
                                       {1, 0},
                                       2.0f,
                                       5.0f,
                                       sf::Color::White,
                                       sf::Color::Transparent,
                                       0.2f,
                                       0.0f};
}

RealEngine::ParticleEmitter ParticlesClass::createShootLoad(sf::Vector2f position) {
    return RealEngine::ParticleEmitter{{},
                                       {position.x, position.y},
                                       {3, 0},
                                       2.0f,
                                       3.5f,
                                       sf::Color::Cyan,
                                       sf::Color{0, 0, 255, 0},
                                       0.25f,
                                       0.0f};
}

RealEngine::ParticleEmitter ParticlesClass::createShootDestroy(sf::Vector2f position) {
    return RealEngine::ParticleEmitter{{},
                                       {position.x, position.y},
                                       {3, 0},
                                       2.0f,
                                       3.5f,
                                       sf::Color::Cyan,
                                       sf::Color{0, 0, 255, 0},
                                       0.25f,
                                       0.0f};
}

Explosion::Explosion(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::ParticleEmitter{{},
                                                               {position.x, position.y},
                                                               {3, 0},
                                                               2.0f,
                                                               5.0f,
                                                               sf::Color::Yellow,
                                                               sf::Color{255, 0, 0, 0},
                                                               2.0f,
                                                               0.0f});
    registry.addComponent(_entity, RealEngine::AutoDestructible{4.0f});
}

Explosion::~Explosion() {}

HitEffect::HitEffect(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::ParticleEmitter{{},
                                                               {position.x, position.y},
                                                               {1, 0},
                                                               2.0f,
                                                               5.0f,
                                                               sf::Color::White,
                                                               sf::Color::Transparent,
                                                               0.2f,
                                                               0.0f});
    registry.addComponent(_entity, RealEngine::AutoDestructible{2.0f});
}

HitEffect::~HitEffect() {}

ShootLoad::ShootLoad(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::ParticleEmitter{{},
                                                               {position.x, position.y},
                                                               {3, 0},
                                                               1.4f,
                                                               1.5f,
                                                               sf::Color::Cyan,
                                                               sf::Color{0, 0, 255, 0},
                                                               0.25f,
                                                               0.0f});
    registry.addComponent(_entity, RealEngine::AutoDestructible{2.0f});
}

ShootLoad::~ShootLoad() {}

}  // namespace rtype