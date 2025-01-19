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
            _particle = create_explosion(position);
            break;
        case HIT_EFFECT:
            _particle = create_hit_effect(position);
            break;
        case SHOOT_LOAD:
            _particle = create_shoot_load(position);
            break;
        default:
            break;
    }
}

ParticlesClass::~ParticlesClass() {}

RealEngine::ParticleEmitter ParticlesClass::create_explosion(sf::Vector2f position) {
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

RealEngine::ParticleEmitter ParticlesClass::create_hit_effect(sf::Vector2f position) {
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

RealEngine::ParticleEmitter ParticlesClass::create_shoot_load(sf::Vector2f position) {
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

RealEngine::ParticleEmitter ParticlesClass::create_shoot_destroy(sf::Vector2f position) {
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
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::ParticleEmitter{{},
                                                                {position.x, position.y},
                                                                {3, 0},
                                                                2.0f,
                                                                5.0f,
                                                                sf::Color::Yellow,
                                                                sf::Color{255, 0, 0, 0},
                                                                2.0f,
                                                                0.0f});
    registry.add_component(_entity, RealEngine::AutoDestructible{4.0f});
}

Explosion::~Explosion() {}

HitEffect::HitEffect(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::ParticleEmitter{{},
                                                                {position.x, position.y},
                                                                {1, 0},
                                                                2.0f,
                                                                5.0f,
                                                                sf::Color::White,
                                                                sf::Color::Transparent,
                                                                0.2f,
                                                                0.0f});
    registry.add_component(_entity, RealEngine::AutoDestructible{2.0f});
}

HitEffect::~HitEffect() {}

ShootLoad::ShootLoad(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::ParticleEmitter{{},
                                                                {position.x, position.y},
                                                                {3, 0},
                                                                1.4f,
                                                                1.5f,
                                                                sf::Color::Cyan,
                                                                sf::Color{0, 0, 255, 0},
                                                                0.25f,
                                                                0.0f});
    registry.add_component(_entity, RealEngine::AutoDestructible{2.0f});
}

ShootLoad::~ShootLoad() {}

}  // namespace rtype