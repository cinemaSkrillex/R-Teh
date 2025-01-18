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
                             RealEngine::Entity entity) {
    auto* autoDestructible = registry.get_component<RealEngine::AutoDestructible>(entity);

    if (collisionType != RealEngine::CollisionType::SCREEN) return;
    if (!autoDestructible) {
        auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
        if (container) {
            try {
                if (std::any_cast<bool>(container->getNetvar("destroy_out_of_screen")->value) ==
                    false) {
                    registry.add_component(entity,
                                           RealEngine::AutoDestructible{-1.0f, true, false});
                }
            } catch (const std::bad_any_cast& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    } else {
        autoDestructible->kill = false;
    }
}

void Block::initialize(RealEngine::Registry& registry, sf::Vector2f position,
                       const std::string& spriteName, float rotation, float scrollingSpeed,
                       RealEngine::CollisionType collisionType) {
    if (position.x < 0 || position.y < 0 || rotation < 0) {
        std::cerr << "Error: Block position is null" << std::endl;
        return;
    }

    registry.add_component(_blockEntity, RealEngine::Position{position.x, position.y});
    registry.add_component(
        _blockEntity, RealEngine::Interpolation{
                          {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.add_component(_blockEntity, RealEngine::SpriteComponent{_blockSprite});
    registry.add_component(_blockEntity, RealEngine::Rotation{rotation});
    registry.add_component(_blockEntity, RealEngine::Drawable{});
    registry.add_component(_blockEntity,
                           RealEngine::Velocity{scrollingSpeed, 0.0f, {0.0f, 0.0f}, 0.0f});
    registry.add_component(_blockEntity,
                           RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                 spriteName,
                                                 false,
                                                 collisionType,
                                                 collisionHandler});
    registry.add_component(
        _blockEntity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string(spriteName), nullptr}},
             {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}},
             {"new_entity", {"bool", "new_entity", true, nullptr}}}});
}

Block::Block(RealEngine::Registry& registry, sf::Vector2f position, const std::string& spriteName,
             float rotation, float scrollingSpeed, RealEngine::CollisionType collisionType)
    : _blockEntity(registry.spawn_entity()),
      _blockSprite(*(RealEngine::AssetManager::getInstance().getSprite(spriteName))),
      _element(spriteName) {
    initialize(registry, position, spriteName, rotation, scrollingSpeed, collisionType);
}
Block::~Block() {}

}  // namespace rtype
