/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Block
*/

#include "Game/Block.hpp"

namespace rtype {

Block::Block(RealEngine::Registry& registry, sf::Vector2f position, const std::string& spriteName,
             float rotation)
    : _blockEntity(registry.spawn_entity()),
      _blockSprite(*(RealEngine::AssetManager::getInstance().getSprite(spriteName))) {
    registry.add_component(_blockEntity, RealEngine::Position{position.x, position.y});
    registry.add_components(_blockEntity, RealEngine::SpriteComponent{_blockSprite});
    registry.add_component(_blockEntity, RealEngine::Rotation{rotation});
    registry.add_component(_blockEntity,
                           RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                 spriteName,
                                                 false,
                                                 RealEngine::CollisionType::SOLID,
                                                 nullptr});
}

Block::~Block() {}

}  // namespace rtype