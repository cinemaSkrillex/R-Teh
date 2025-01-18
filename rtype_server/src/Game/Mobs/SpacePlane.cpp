/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpacePlane
*/

#include "Game/Mobs/SpacePlane.hpp"

namespace rtype {

static void straight_line_behavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                                   float deltaTime) {
    auto* position = registry.get_component<RealEngine::Position>(entity);
    auto* velocity = registry.get_component<RealEngine::Velocity>(entity);

    if (position->x > 0) {
        velocity->vx = -100.0f;
    }
}

SpacePlane::SpacePlane(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()),
      _mobSprite(*(RealEngine::AssetManager::getInstance().getSprite("space_plane"))) {
    _mobSpriteSheet.emplace("normal", _mobSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {500.f, 500.f}, 0.5f});
    registry.add_component(
        _entity,
        RealEngine::SpriteSheet{
            _mobSpriteSheet, "normal", 0, {21, 23}, false, true, 75, {10, 12}, sf::Clock()});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});
    registry.add_component(_entity, RealEngine::AI{noBehavior, straight_line_behavior, true});
    registry.add_component(_entity, RealEngine::Damage{50});
    registry.add_component(_entity, RealEngine::Health{40, 40});
    registry.add_component(_entity, RealEngine::Rotation{0.f});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
        _entity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("space_plane"), nullptr}},
             {"entity_type", {"int", "entity_type", 2, nullptr}}}});
}

SpacePlane::~SpacePlane() {}

}  // namespace rtype