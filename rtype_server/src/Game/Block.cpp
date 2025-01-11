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
      _blockSprite(*(RealEngine::AssetManager::getInstance().getSprite(spriteName))),
      _element(spriteName) {
    if (position.x < 0 || position.y < 0 || rotation < 0) {
        std::cerr << "Error: Block position is null" << std::endl;
        return;
    }

    registry.add_component(_blockEntity, RealEngine::Position{position.x, position.y});
    registry.add_component(_blockEntity, RealEngine::SpriteComponent{_blockSprite});
    registry.add_component(_blockEntity, RealEngine::Rotation{rotation});
    registry.add_component(_blockEntity, RealEngine::Drawable{});
    registry.add_component(_blockEntity,
                           RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                 spriteName,
                                                 false,
                                                 RealEngine::CollisionType::SOLID,
                                                 nullptr});
    registry.add_component(_blockEntity, RealEngine::BlockTag{});  // Add BlockTag component
}

Block::~Block() {}

}  // namespace rtype