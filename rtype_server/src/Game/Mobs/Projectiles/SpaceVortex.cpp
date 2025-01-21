/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpaceVortex
*/

#include "Game/Mobs/Projectiles/SpaceVortex.hpp"

namespace rtype {

SpaceVortex::SpaceVortex(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawnEntity()),
      _projSprite(*(RealEngine::AssetManager::getInstance().getSprite("space_vortex"))) {
    _projSpriteSheet.emplace("normal", _projSprite);
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.addComponent(_entity, RealEngine::Velocity{100.0f, 0, {850.f, 850.f}, 0.5f});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("space_vortex");
    registry.addComponent(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.addComponent(_entity,
                          RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                "mob",
                                                false,
                                                RealEngine::CollisionType::ENEMY_BULLET,
                                                noCollisionBehavior});
    registry.addComponent(_entity, RealEngine::AI{noBehavior, noBehavior, true});
    registry.addComponent(_entity, RealEngine::Damage{50});
    registry.addComponent(_entity, RealEngine::Health{1, 1});
    registry.addComponent(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.addComponent(
        _entity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("space_vortex"), nullptr}}}});
}

SpaceVortex::~SpaceVortex() {}

}  // namespace rtype