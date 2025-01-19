/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** WallTurret
*/

#include "Game/Mobs/WallTurret.hpp"

namespace rtype {

static void canonBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                          float deltaTime) {
    auto target         = registry.get_component<RealEngine::Target>(entity);
    auto targetPosition = registry.get_component<RealEngine::Position>(target->target);
    moveWithoutVelocity(registry, entity, deltaTime);
    aimAtTarget(entity, targetPosition, registry, 0.75f, deltaTime);
}

WallTurret::WallTurret(RealEngine::Registry& registry, sf::Vector2f position, bool onGround,
                       float speed)
    : _canonEntity(registry.spawn_entity()),
      _pedestalEntity(registry.spawn_entity()),
      _canonSprite(*(RealEngine::AssetManager::getInstance().getSprite("turret_canon"))),
      _pedestalSprite(*(RealEngine::AssetManager::getInstance().getSprite("turret_pedestal"))) {
    _canonSprite.setOrigin(5, 5);
    registry.add_component(_canonEntity, RealEngine::Position{position.x, position.y});
    registry.add_component(_canonEntity, RealEngine::Acceleration{speed, 0.f, 0.f});
    registry.add_component(_canonEntity, RealEngine::SpriteComponent{_canonSprite});
    registry.add_component(_canonEntity, RealEngine::Drawable{});
    registry.add_component(_canonEntity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});
    registry.add_component(_canonEntity, RealEngine::AI{canonBehavior, noBehavior, true});
    registry.add_component(_canonEntity, RealEngine::Damage{10});
    registry.add_component(_canonEntity, RealEngine::Health{40, 40});
    registry.add_component(_canonEntity, RealEngine::Rotation{0.f});
    registry.add_component(_canonEntity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
        _canonEntity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("turret_canon"), nullptr}}}});

    registry.add_component(_pedestalEntity,
                           RealEngine::Position{position.x, position.y + onGround ? -5.f : 5.f});
    registry.add_component(_pedestalEntity, RealEngine::Acceleration{speed, 0.f, 0.f});
    registry.add_component(_pedestalEntity, RealEngine::SpriteComponent{_pedestalSprite});
    registry.add_component(_pedestalEntity, RealEngine::Drawable{});
    registry.add_component(_pedestalEntity, RealEngine::Rotation{onGround ? 0.f : 180.f});
    registry.add_component(_pedestalEntity, RealEngine::AI{noBehavior, moveWithoutVelocity, true});
    registry.add_component(_pedestalEntity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
        _pedestalEntity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("turret_pedestal"), nullptr}}}});
}

WallTurret::~WallTurret() {}

void WallTurret::setTarget(std::shared_ptr<RealEngine::Entity> target,
                           RealEngine::Registry&               registry) {
    registry.add_component(_canonEntity, RealEngine::Target{target});
}

}  // namespace rtype