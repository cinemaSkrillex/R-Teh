#include "EyeBoss.hpp"

namespace rtype {
EyeBoss::EyeBoss(RealEngine::Registry& registry)
    : _state(EyeBossState::SHORT_RANGE),
      _registry(registry),
      _entity(registry.spawn_entity()),
      _shortSprite("../../assets/sprites/the_eye/boss.png", sf::IntRect{0, 0, 73 * 3, 55}),
      _midSprite("../../assets/sprites/the_eye/boss.png", sf::IntRect{0, 55, 91 * 3, 55}),
      _longSprite("../../assets/sprites/the_eye/boss.png", sf::IntRect{0, 55 * 2, 81 * 3, 55}),
      _laserSprite("../../assets/sprites/the_eye/laser_shoot.png", sf::IntRect{0, 0, 30, 7}),
      _shootCooldown(0.0f),
      _shootPhaseTimer(0.0f),
      _isInShootPhase(false) {
    _shortSprite.setScale(GAME_SCALE, GAME_SCALE);
    _midSprite.setScale(GAME_SCALE, GAME_SCALE);
    _longSprite.setScale(GAME_SCALE, GAME_SCALE);
    _laserSprite.setScale(GAME_SCALE, GAME_SCALE);
    _bossSheet.emplace("short", _shortSprite);
    _bossSheet.emplace("mid", _midSprite);
    _bossSheet.emplace("long", _longSprite);

    _registry.add_components(_entity, RealEngine::Position{200.f, 200.f}, RealEngine::Drawable{});
    _registry.add_component(
        _entity,
        RealEngine::SpriteSheet{_bossSheet, "short", 0, {73, 55}, false, true, 120, {48, 26}});
    _registry.add_component(_entity, RealEngine::Velocity{0.0f, 0.0f, {400.0f, 400.0f}, 1.8f});
    _registry.add_component(_entity, RealEngine::Acceleration{2.0f, 2.0f, 2.0f});
    _registry.add_component(_entity, RealEngine::Rotation{300.0f});
    _registry.add_component(
        _entity,
        RealEngine::Radius{800.0f, 350.0f, [this]() { setBossStatus(1); },
                           [this]() { setBossStatus(2); }, [this]() { setBossStatus(0); }});
    _registry.add_component(
        _entity,
        RealEngine::AI{[this](RealEngine::Registry& registry, RealEngine::Entity target,
                              float deltaTime) { targetBossBehavior(registry, target, deltaTime); },
                       [this](RealEngine::Registry& registry, float deltaTime) {
                           noTargetBossBehavior(registry, deltaTime);
                       },
                       true});
}

EyeBoss::~EyeBoss() {}

void EyeBoss::setTarget(RealEngine::Entity target) {
    _registry.add_component(_entity, RealEngine::Target{target});
}

void EyeBoss::targetBossBehavior(RealEngine::Registry& registry, RealEngine::Entity target,
                                 float deltaTime) {
    float rotationSpeed = 0.1f;
    switch (_state) {
        case EyeBossState::SHORT_RANGE:
            shortRangeBehavior(registry, target);
            rotationSpeed = 1.0f;
            break;
        case EyeBossState::MID_RANGE:
            if (_shootPhaseTimer > 0.0f) {
                _shootPhaseTimer -= 1.0f * deltaTime;
            } else {
                _shootPhaseTimer = 2.0f;
                _isInShootPhase  = !_isInShootPhase;
            }
            if (_shootCooldown > 0.0f) {
                _shootCooldown -= 1.0f * deltaTime;
            }
            midRangeBehavior(registry, target);
            rotationSpeed = 0.6f;
            break;
        case EyeBossState::LONG_RANGE:
            circularAttack(registry, target, deltaTime);
            // longRangeBehavior(registry, target);
            // rotationSpeed = 2.2f;
            break;
        default:
            break;
    }
    aimAtTarget(registry.get_component<RealEngine::Position>(target), rotationSpeed, deltaTime);
}

void EyeBoss::aimAtTarget(RealEngine::Position* targetPosition, float rotationSpeed,
                          float deltaTime) {
    auto* position = _registry.get_component<RealEngine::Position>(_entity);
    auto* rotation = _registry.get_component<RealEngine::Rotation>(_entity);

    if (position && targetPosition) {
        float dx          = targetPosition->x - position->x;
        float dy          = targetPosition->y - position->y;
        float targetAngle = std::atan2(dy, dx) * 180.0f / M_PI;

        if (rotation) {
            float currentAngle    = rotation->angle;
            targetAngle           = std::fmod(targetAngle + 360.0f, 360.0f);
            currentAngle          = std::fmod(currentAngle + 360.0f, 360.0f);
            float angleDifference = targetAngle - currentAngle;

            if (angleDifference > 180.0f) {
                angleDifference -= 360.0f;
            } else if (angleDifference < -180.0f) {
                angleDifference += 360.0f;
            }
            if (angleDifference > 0.1f) {
                rotation->angle += rotationSpeed * deltaTime * 100.0f;
            } else if (angleDifference < -0.1f) {
                rotation->angle -= rotationSpeed * deltaTime * 100.0f;
            }
            rotation->angle = std::fmod(rotation->angle + 360.0f, 360.0f);
        }
    }
}

void EyeBoss::noTargetBossBehavior(RealEngine::Registry& registry, float deltaTime) {
    // Do something
}

void EyeBoss::shortRangeBehavior(RealEngine::Registry& registry, RealEngine::Entity target) {
    auto* position       = registry.get_component<RealEngine::Position>(_entity);
    auto* acceleration   = registry.get_component<RealEngine::Acceleration>(_entity);
    auto* velocity       = registry.get_component<RealEngine::Velocity>(_entity);
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

void EyeBoss::midRangeBehavior(RealEngine::Registry& registry, RealEngine::Entity target) {
    auto* position     = registry.get_component<RealEngine::Position>(_entity);
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(_entity);
    auto* velocity     = registry.get_component<RealEngine::Velocity>(_entity);
    auto* rotation     = registry.get_component<RealEngine::Rotation>(_entity);

    if (abs(velocity->vx) < 25.0f && abs(velocity->vy) < 25.0f) {
        if (position && acceleration && rotation) {
            float angleRad   = rotation->angle * M_PI / 180.0f;
            acceleration->ax = std::cos(angleRad) * 250.0f;
            acceleration->ay = std::sin(angleRad) * 250.0f;
            velocity->vx += acceleration->ax;
            velocity->vy += acceleration->ay;
        }
    }
    if (_shootCooldown <= 0.0f) {
        shootLaser();
        _shootCooldown = 0.75f;
    }
    if (_isInShootPhase && _shootCooldown > 0.1f) {
        _shootCooldown = 0.1f;
    }
}

void EyeBoss::longRangeBehavior(RealEngine::Registry& registry, RealEngine::Entity target) {
    auto* position       = registry.get_component<RealEngine::Position>(_entity);
    auto* acceleration   = registry.get_component<RealEngine::Acceleration>(_entity);
    auto* velocity       = registry.get_component<RealEngine::Velocity>(_entity);
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

void EyeBoss::shootLaser() {
    RealEngine::Entity laser         = _registry.spawn_entity();
    auto*              boss_position = _registry.get_component<RealEngine::Position>(_entity);
    auto*              boss_rotation = _registry.get_component<RealEngine::Rotation>(_entity);
    float              angleRad      = boss_rotation->angle * M_PI / 180.0f;

    _registry.add_components(
        laser,
        RealEngine::Position{boss_position->x + (std::cos(angleRad) * (56 * GAME_SCALE)),
                             boss_position->y + (std::sin(angleRad) * (56 * GAME_SCALE))},
        RealEngine::Drawable{});
    _registry.add_component(laser, RealEngine::SpriteComponent{_laserSprite});
    _registry.add_component(
        laser,
        RealEngine::Velocity{
            std::cos(angleRad) * 500.0f, std::sin(angleRad) * 500.0f, {500.0f, 500.0f}, 0.0f});
    _registry.add_component(laser, RealEngine::Rotation{boss_rotation->angle});
    _registry.add_component(laser, RealEngine::AutoDestructible{lifeTime : 2.0f});
    _laserEntities.push_back(laser);
}

void EyeBoss::circularAttack(RealEngine::Registry& registry, RealEngine::Entity target,
                             float deltaTime) {
    auto* rotation = registry.get_component<RealEngine::Rotation>(_entity);

    if (rotation) {
        rotation->angle += 200.0f * deltaTime;
    }
    if (_shootCooldown > 0.0f) {
        _shootCooldown -= 1.0f * deltaTime;
    } else {
        shootLaser();
        _shootCooldown = 0.005f;
    }
}

void EyeBoss::setBossStatus(int state) {
    auto* spriteSheet = _registry.get_component<RealEngine::SpriteSheet>(_entity);

    switch (state) {
        case 0:
            _state                   = EyeBossState::SHORT_RANGE;
            spriteSheet->spriteIndex = "short";
            spriteSheet->frameSize   = {73, 55};
            break;
        case 1:
            _state                   = EyeBossState::MID_RANGE;
            spriteSheet->spriteIndex = "mid";
            spriteSheet->frameSize   = {91, 55};
            break;
        case 2:
            _state                   = EyeBossState::LONG_RANGE;
            spriteSheet->spriteIndex = "long";
            spriteSheet->frameSize   = {81, 55};
            break;
        default:
            break;
    }
}
}  // namespace rtype