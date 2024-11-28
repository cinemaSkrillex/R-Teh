#include "EyeBoss.hpp"

namespace rtype {
EyeBoss::EyeBoss(RealEngine::Registry& registry)
    : _state(EyeBossState::SHORT_RANGE),
      _registry(registry),
      _entity(registry.spawn_entity()),
      _shortSprite("../assets/sprites/the_eye/boss.png", sf::IntRect{0, 0, 73 * 3, 55}),
      _midSprite("../assets/sprites/the_eye/boss.png", sf::IntRect{0, 55, 91 * 3, 55}),
      _longSprite("../assets/sprites/the_eye/boss.png", sf::IntRect{0, 55 * 2, 81 * 3, 55}) {
    _shortSprite.setScale(3, 3);
    _midSprite.setScale(3, 3);
    _longSprite.setScale(3, 3);
    _bossSheet.emplace("short", _shortSprite);
    _bossSheet.emplace("mid", _midSprite);
    _bossSheet.emplace("long", _longSprite);
    _registry.add_components(_entity, RealEngine::Position{200.f, 200.f}, RealEngine::Drawable{});
    setBossStatus(0);
    _registry.add_component(_entity, RealEngine::Rotation{300.0f});
    _registry.add_component(
        _entity,
        RealEngine::Radius{800.0f, 200.0f, [this]() { setBossStatus(1); },
                           [this]() { setBossStatus(2); }, [this]() { setBossStatus(0); }});
    _registry.add_component(
        _entity,
        RealEngine::AI{[this](RealEngine::Registry& registry, RealEngine::Entity target,
                              float deltaTime) { targetBossBehavior(registry, target, deltaTime); },
                       [this](RealEngine::Registry& registry, float deltaTime) {
                           PassiveBossBehavior(registry, deltaTime);
                       },
                       true});
}

EyeBoss::~EyeBoss() {}

void EyeBoss::setTarget(RealEngine::Entity target) {
    _registry.add_component(_entity, RealEngine::Target{target});
}

void EyeBoss::targetBossBehavior(RealEngine::Registry& registry, RealEngine::Entity target,
                                 float deltaTime) {
    auto* position       = registry.get_component<RealEngine::Position>(_entity);
    auto* targetPosition = registry.get_component<RealEngine::Position>(target);
    float rotationSpeed  = 0.1f;
    switch (_state) {
        case EyeBossState::SHORT_RANGE:
            shortRangeBehavior();
            rotationSpeed = 5.0f;
            break;
        case EyeBossState::MID_RANGE:
            midRangeBehavior();
            rotationSpeed = 4.2f;
            break;
        case EyeBossState::LONG_RANGE:
            longRangeBehavior();
            rotationSpeed = 2.2f;
            break;
        default:
            break;
    }

    if (position && targetPosition) {
        float dx          = targetPosition->x - position->x;
        float dy          = targetPosition->y - position->y;
        float targetAngle = std::atan2(dy, dx) * 180 / M_PI;

        auto* rotation = registry.get_component<RealEngine::Rotation>(_entity);
        if (rotation) {
            float currentAngle    = rotation->angle;
            float angleDifference = targetAngle - currentAngle;

            if (angleDifference > 180.0f) {
                angleDifference -= 360.0f;
            } else if (angleDifference < -180.0f) {
                angleDifference += 360.0f;
            }

            if (angleDifference > 0.1f) {
                rotation->angle += rotationSpeed * deltaTime * 100;
            } else if (angleDifference < -0.1f) {
                rotation->angle -= rotationSpeed * deltaTime * 100;
            }
        }
    }
}

void EyeBoss::PassiveBossBehavior(RealEngine::Registry& registry, float deltaTime) {
    auto* rotation = registry.get_component<RealEngine::Rotation>(_entity);

    if (rotation) {
        rotation->angle += 0.1f;
    }
    // Do something
}

void EyeBoss::shortRangeBehavior() {
    // Do something
}

void EyeBoss::midRangeBehavior() {
    // Do something
}

void EyeBoss::longRangeBehavior() {
    // Do something
}

void EyeBoss::setBossStatus(int state) {
    switch (state) {
        case 0:
            _state = EyeBossState::SHORT_RANGE;
            _registry.add_component(
                _entity,
                RealEngine::SpriteSheet{_bossSheet, "short", 0, {73, 55}, false, true, 120});
            break;
        case 1:
            _state = EyeBossState::MID_RANGE;
            _registry.add_component(
                _entity, RealEngine::SpriteSheet{_bossSheet, "mid", 0, {91, 55}, false, true, 120});
            break;
        case 2:
            _state = EyeBossState::LONG_RANGE;
            _registry.add_component(
                _entity,
                RealEngine::SpriteSheet{_bossSheet, "long", 0, {81, 55}, false, true, 120});
            break;
        default:
            break;
    }
}
}  // namespace rtype