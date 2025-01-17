/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** WaitingBlock
*/

#include "Game/Level/WaitingBlock.hpp"

namespace rtype {

void WaitingBlock::waitingBlockCollisionHandler(RealEngine::CollisionType collisionType,
                                                RealEngine::Registry&     registry,
                                                RealEngine::Entity        collider,
                                                RealEngine::Entity        entity) {
    switch (collisionType) {
        case RealEngine::CollisionType::PLAYER:
            _playersInBox++;
            break;
    }
}

void WaitingBlock::initialize(RealEngine::Registry& registry, sf::Vector2f position,
                              const std::string& spriteName, float rotation, float scrollingSpeed,
                              RealEngine::CollisionType collisionType) {
    (void)scrollingSpeed;
    if (position.x < 0 || position.y < 0 || rotation < 0) {
        std::cerr << "Error: WaitingBlock position is null" << std::endl;
        return;
    }
    registry.add_component(_waitingEntity, RealEngine::Position{position.x, position.y});
    registry.add_component(
        _waitingEntity, RealEngine::Interpolation{
                            {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.add_component(_waitingEntity, RealEngine::SpriteComponent{_waitingBlockSprite});
    registry.add_component(_waitingEntity, RealEngine::Rotation{rotation});
    registry.add_component(_waitingEntity, RealEngine::Drawable{});
    registry.add_component(
        _waitingEntity,
        RealEngine::Collision{
            {0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
            spriteName,
            false,
            RealEngine::CollisionType::OTHER,
            std::function<void(RealEngine::CollisionType, RealEngine::Registry&, RealEngine::Entity,
                               RealEngine::Entity)>(
                std::bind(&WaitingBlock::waitingBlockCollisionHandler, this, std::placeholders::_1,
                          std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))});
    registry.add_component(
        _waitingEntity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string(spriteName), nullptr}},
             {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}},
             {"new_entity", {"bool", "new_entity", true, nullptr}},
             {"change_scene_timer", {"float", "goToLevel", float(0.f), nullptr}}}});
}

WaitingBlock::WaitingBlock(RealEngine::Registry& registry, sf::Vector2f position,
                           const std::string& spriteName, float rotation,
                           RealEngine::CollisionType collisionType)
    : _waitingEntity(registry.spawn_entity()),
      _waitingBlockSprite(*(RealEngine::AssetManager::getInstance().getSprite(spriteName))),
      _element(spriteName),
      _playersInBox(0) {
    initialize(registry, position, spriteName, rotation, 0, collisionType);
}

WaitingBlock::~WaitingBlock() { std::cout << "WaitingBlock destroyed" << std::endl; }
}  // namespace rtype