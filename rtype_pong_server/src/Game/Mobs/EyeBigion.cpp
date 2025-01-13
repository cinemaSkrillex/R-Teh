/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBigion
*/

#include "Game/Mobs/EyeBigion.hpp"

namespace rtype {

static void agressiveBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                              float deltaTime) {
    rushTowardsTarget(registry, entity, deltaTime);
}

EyeBigion::EyeBigion(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()) {
    _spriteSheet.emplace("normal",
                         *(RealEngine::AssetManager::getInstance().getSprite("eye_bigion_normal")));
    _spriteSheet.emplace("angry",
                         *(RealEngine::AssetManager::getInstance().getSprite("eye_bigion_angry")));

    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_components(
        _entity,
        RealEngine::SpriteSheet{
            _spriteSheet, "normal", 0, {23, 16}, false, true, 120, {17, 8}, sf::Clock()},
        RealEngine::Drawable{});
    // 23, 16 size and 17, 8 center for normal form
    // 21, 16 size and 16, 8 center for angry form
    registry.add_component(_entity, RealEngine::Velocity{0.0f, 0.0f, {135.0f, 135.0f}, 0.8f});
    registry.add_component(_entity, RealEngine::Acceleration{60.0f, 5.0f, 0.5f});
    registry.add_component(_entity, RealEngine::Rotation{0.0f});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});
    registry.add_component(_entity, RealEngine::Health{100, 100});
    registry.add_component(_entity, RealEngine::AI{rushAndAimTowardsTarget, noBehavior, true});
    registry.add_component(_entity, RealEngine::Damage{40});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
        _entity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("eye_bigion"), nullptr}}}});
}

EyeBigion::~EyeBigion() {}

void EyeBigion::setTarget(std::shared_ptr<RealEngine::Entity> target,
                          RealEngine::Registry&               registry) {
    registry.add_component(_entity, RealEngine::Target{target});
}

}  // namespace rtype