/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Block
*/

#include "../../../include/Game/Level/Block.hpp"

namespace rtype {

static void collisionHandler(RealEngine::CollisionType collisionType,
                             RealEngine::Registry& registry, RealEngine::Entity collider,
                             RealEngine::Entity entity) {}

void Block::initialize(RealEngine::Registry& registry, sf::Vector2f position,
                       const std::string& spriteName, float rotation, float scrollingSpeed,
                       RealEngine::CollisionType collisionType) {
    if (position.x < 0 || position.y < 0 || rotation < 0) {
        std::cerr << "initialize: Error: Block position is null" << std::endl;
        return;
    }

    registry.addComponent(_blockEntity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_blockEntity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.addComponent(_blockEntity, RealEngine::SpriteComponent{_blockSprite});
    registry.addComponent(_blockEntity, RealEngine::Rotation{rotation});
    registry.addComponent(_blockEntity, RealEngine::Drawable{});
    std::cout << "scrolling speed of block BLOCK: " << scrollingSpeed << std::endl;
    registry.addComponent(_blockEntity,
                          RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                spriteName,
                                                false,
                                                collisionType,
                                                collisionHandler});
    registry.addComponent(
        _blockEntity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string(spriteName), nullptr}},
             {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}},
             {"new_entity", {"bool", "new_entity", true, nullptr}}}});
}

Block::Block(RealEngine::Registry& registry, sf::Vector2f position, const std::string& spriteName,
             float rotation, float scrollingSpeed, RealEngine::CollisionType collisionType)
    : _blockEntity(registry.spawnEntity()),
      _blockSprite(*(RealEngine::AssetManager::getInstance().getSprite(spriteName))),
      _element(spriteName) {
    Block::initialize(registry, position, spriteName, rotation, scrollingSpeed, collisionType);
}
Block::~Block() {}

}  // namespace rtype
