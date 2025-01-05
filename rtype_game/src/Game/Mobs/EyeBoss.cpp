/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBoss
*/

#include "Game/Mobs/EyeBoss.hpp"

// this mob is a WIP commented lines needs old class variables that will be replaced with netvars

namespace rtype {

static void noTargetBossBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                                 float deltaTime) {
    // Do something
}

static void circularAttack(RealEngine::Registry& registry, RealEngine::Entity& entity,
                           RealEngine::Entity target, float deltaTime) {
    auto* rotation = registry.get_component<RealEngine::Rotation>(entity);

    if (rotation) {
        rotation->angle += 200.0f * deltaTime;
    }
    // if (_shootCooldown > 0.0f) {
    //     _shootCooldown -= 1.0f * deltaTime;
    // } else {
    //     shootLaser();
    //     _shootCooldown = 0.005f;
    // }
}

static void shortRangeBehavior(RealEngine::Registry& registry, RealEngine::Entity& entity,
                               RealEngine::Entity target) {
    auto* position       = registry.get_component<RealEngine::Position>(entity);
    auto* acceleration   = registry.get_component<RealEngine::Acceleration>(entity);
    auto* velocity       = registry.get_component<RealEngine::Velocity>(entity);
    auto* targetPosition = registry.get_component<RealEngine::Position>(target);

    if (position && targetPosition && acceleration) {
        float dx       = targetPosition->x - position->x;
        float dy       = targetPosition->y - position->y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 10.0f) {
            acceleration->ax = dx / distance * 6.0f;
            acceleration->ay = dy / distance * 6.0f;
            velocity->vx += acceleration->ax;
            velocity->vy += acceleration->ay;
        } else {
            acceleration->ax = 0.0f;
            acceleration->ay = 0.0f;
        }
    }
}

static void midRangeBehavior(RealEngine::Registry& registry, RealEngine::Entity& entity,
                             RealEngine::Entity target) {
    auto* position     = registry.get_component<RealEngine::Position>(entity);
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(entity);
    auto* velocity     = registry.get_component<RealEngine::Velocity>(entity);
    auto* rotation     = registry.get_component<RealEngine::Rotation>(entity);

    if (abs(velocity->vx) < 25.0f && abs(velocity->vy) < 25.0f) {
        if (position && acceleration && rotation) {
            float angleRad   = rotation->angle * M_PI / 180.0f;
            acceleration->ax = std::cos(angleRad) * 250.0f;
            acceleration->ay = std::sin(angleRad) * 250.0f;
            velocity->vx += acceleration->ax;
            velocity->vy += acceleration->ay;
        }
    }
    // if (_shootCooldown <= 0.0f) {
    //     shootLaser();
    //     _shootCooldown = 0.75f;
    // }
    // if (_isInShootPhase && _shootCooldown > 0.1f) {
    //     _shootCooldown = 0.1f;
    // }
}

static void longRangeBehavior(RealEngine::Registry& registry, RealEngine::Entity& entity,
                              RealEngine::Entity target) {
    auto* position       = registry.get_component<RealEngine::Position>(entity);
    auto* acceleration   = registry.get_component<RealEngine::Acceleration>(entity);
    auto* velocity       = registry.get_component<RealEngine::Velocity>(entity);
    auto* targetPosition = registry.get_component<RealEngine::Position>(target);

    if (position && targetPosition && acceleration) {
        float dx       = targetPosition->x - position->x;
        float dy       = targetPosition->y - position->y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 10.0f) {
            acceleration->ax = dx / distance * 2.0f;
            velocity->vx += acceleration->ax;
            acceleration->ay = dy / distance * 2.0f;
            velocity->vy += acceleration->ay;
        } else {
            acceleration->ax = 0.0f;
            acceleration->ay = 0.0f;
        }
    }
}

static void targetBossBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                               float deltaTime) {
    auto  target        = registry.get_component<RealEngine::Target>(entity)->target;
    float rotationSpeed = 0.1f;
    // switch (_state) {
    //     case EyeBossState::SHORT_RANGE:
    //         shortRangeBehavior(registry, entity, target);
    //         rotationSpeed = 1.0f;
    //         break;
    //     case EyeBossState::MID_RANGE:
    //         // if (_shootPhaseTimer > 0.0f) {
    //         //     _shootPhaseTimer -= 1.0f * deltaTime;
    //         // } else {
    //         //     _shootPhaseTimer = 2.0f;
    //         //     _isInShootPhase  = !_isInShootPhase;
    //         // }
    //         // if (_shootCooldown > 0.0f) {
    //         //     _shootCooldown -= 1.0f * deltaTime;
    //         // }
    //         midRangeBehavior(registry, entity, target);
    //         rotationSpeed = 0.6f;
    //         break;
    //     case EyeBossState::LONG_RANGE:
    //         circularAttack(registry, entity, target, deltaTime);
    //         // longRangeBehavior(registry, target);
    //         // rotationSpeed = 2.2f;
    //         break;
    //     default:
    //         break;
    // }
    aimAtTarget(entity, registry.get_component<RealEngine::Position>(target), registry,
                rotationSpeed, deltaTime);
}

static void setBossStatusShort(RealEngine::Registry& registry, RealEngine::Entity entity) {
    auto* spriteSheet = registry.get_component<RealEngine::SpriteSheet>(entity);

    // _state                   = EyeBossState::SHORT_RANGE;
    spriteSheet->spriteIndex = "short";
    spriteSheet->frameSize   = {73, 55};
}

static void setBossStatusMid(RealEngine::Registry& registry, RealEngine::Entity entity) {
    auto* spriteSheet = registry.get_component<RealEngine::SpriteSheet>(entity);

    // _state                   = EyeBossState::MID_RANGE;
    spriteSheet->spriteIndex = "mid";
    spriteSheet->frameSize   = {91, 55};
}

static void setBossStatusLong(RealEngine::Registry& registry, RealEngine::Entity entity) {
    auto* spriteSheet = registry.get_component<RealEngine::SpriteSheet>(entity);

    // _state                   = EyeBossState::LONG_RANGE;
    spriteSheet->spriteIndex = "long";
    spriteSheet->frameSize   = {81, 55};
}

EyeBoss::EyeBoss(RealEngine::Registry& registry)
    : _state(EyeBossState::SHORT_RANGE),
      _entity(registry.spawn_entity()),
      _shootCooldown(0.0f),
      _shootPhaseTimer(0.0f),
      _isInShootPhase(false) {
    _bossSheet.emplace("short",
                       *(RealEngine::AssetManager::getInstance().getSprite("eye_boss_short")));
    _bossSheet.emplace("mid", *(RealEngine::AssetManager::getInstance().getSprite("eye_boss_mid")));
    _bossSheet.emplace("long",
                       *(RealEngine::AssetManager::getInstance().getSprite("eye_boss_long")));

    registry.add_components(_entity, RealEngine::Position{800.f, 800.f}, RealEngine::Drawable{});
    registry.add_component(
        _entity, RealEngine::SpriteSheet{
                     _bossSheet, "short", 0, {73, 55}, false, true, 120, {48, 26}, sf::Clock()});
    registry.add_component(_entity, RealEngine::Velocity{0.0f, 0.0f, {400.0f, 400.0f}, 1.8f});
    registry.add_component(_entity, RealEngine::Acceleration{2.0f, 2.0f, 2.0f});
    registry.add_component(_entity, RealEngine::Rotation{300.0f});
    registry.add_component(_entity, RealEngine::Radius{800.0f, 350.0f, setBossStatusShort,
                                                       setBossStatusMid, setBossStatusLong});
    registry.add_component(_entity, RealEngine::AI{targetBossBehavior, noTargetBossBehavior,
                                                   false});  // the boss ai is disabled for now
}

EyeBoss::~EyeBoss() {}

void EyeBoss::setTarget(RealEngine::Registry&               registry,
                        std::shared_ptr<RealEngine::Entity> target) {
    registry.add_component(_entity, RealEngine::Target{target});
}

// void EyeBoss::shootLaser() {
//     // adjust parameters from the boss (angle speed etc)
//     EyeLaser laser(_registry, {0.0f, 0.0f}, 0.0f, 0.0f);
// }

}  // namespace rtype