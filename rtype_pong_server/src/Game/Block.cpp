/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Block
*/

#include "Game/Block.hpp"

namespace rtype {

static void collisionHandler(RealEngine::CollisionType collisionType,
                             RealEngine::Registry& registry, RealEngine::Entity collider,
                             RealEngine::Entity entity) {
    return;
    auto* autoDestructible = registry.get_component<RealEngine::AutoDestructible>(entity);

    if (collisionType != RealEngine::CollisionType::SCREEN) return;
    if (!autoDestructible) {
        auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
        if (container) {
            bool destroy_out_of_screen =
                std::any_cast<bool>(container->getNetvar("destroy_out_of_screen")->value);
            if (!destroy_out_of_screen) {
                registry.add_component(entity, RealEngine::AutoDestructible{-1.0f, true, false});
            }
        }
    }
    autoDestructible->kill = false;
}

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
                                                 collisionHandler});
    registry.add_component(
        _blockEntity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string(spriteName), nullptr}},
             {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}}}});
}

Block::~Block() {}

}  // namespace rtype