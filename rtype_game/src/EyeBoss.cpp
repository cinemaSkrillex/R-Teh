#include "EyeBoss.hpp"

namespace rtype {
EyeBoss::EyeBoss(RealEngine::Registry& registry)
    : _state(EyeBossState::SHORT_RANGE),
      _registry(registry),
      _entity(registry.spawn_entity()),
      _shortSprite("../assets/sprites/the_eye/boss.png", sf::IntRect{0, 0, 55 * 3, 73}),
      _midSprite("../assets/sprites/the_eye/boss.png", sf::IntRect{0, 73, 55 * 3, 91}),
      _longSprite("../assets/sprites/the_eye/boss.png", sf::IntRect{0, 73 + 91, 55 * 3, 81}) {
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
}

EyeBoss::~EyeBoss() {}

void EyeBoss::bossBehavior() {
    switch (_state) {
        case EyeBossState::SHORT_RANGE:
            shortRangeBehavior();
            break;
        case EyeBossState::MID_RANGE:
            midRangeBehavior();
            break;
        case EyeBossState::LONG_RANGE:
            longRangeBehavior();
            break;
        default:
            break;
    }
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
                RealEngine::SpriteSheet{_bossSheet, "short", 0, {55, 73}, false, true, 120});
            break;
        case 1:
            _state = EyeBossState::MID_RANGE;
            _registry.add_component(
                _entity, RealEngine::SpriteSheet{_bossSheet, "mid", 0, {55, 91}, false, true, 120});
            break;
        case 2:
            _state = EyeBossState::LONG_RANGE;
            _registry.add_component(
                _entity,
                RealEngine::SpriteSheet{_bossSheet, "long", 0, {55, 81}, false, true, 120});
            break;
        default:
            break;
    }
}
}  // namespace rtype