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
    // _registry.add_components(_entity, RealEngine::Position{100.0f, 100.0f});
    // _registry.add_component(_entity, RealEngine::Drawable{});
    // _registry.add_component(_entity, RealEngine::SpriteComponent{_shortSprite});
    _registry.add_components(_entity, RealEngine::Position{200.f, 200.f}, RealEngine::Drawable{});
    _registry.add_component(
        _entity, RealEngine::SpriteSheet{_bossSheet, "short", 0, {55, 73}, false, true, 120});
    // _registry.add_component(_entity, RealEngine::SpriteComponent{_shortSprite});
    _registry.add_component(_entity, RealEngine::Rotation{300.0f});
}

EyeBoss::~EyeBoss() {}

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
            break;
        case 1:
            _state = EyeBossState::MID_RANGE;
            break;
        case 2:
            _state = EyeBossState::LONG_RANGE;
            break;
        default:
            break;
    }
}
}  // namespace rtype