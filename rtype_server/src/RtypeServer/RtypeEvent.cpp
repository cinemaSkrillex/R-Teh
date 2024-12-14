/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeEvent
*/

#include "../../include/RtypeServer.hpp"

void RtypeServer::runEvent(const std::unordered_map<std::string, std::string>& parsed_data,
                           asio::ip::udp::endpoint& client, Player& player) {
    if (parsed_data.at("Event") == "Shoot") {
        _game_instance->addBullet(player.getPosition(), {1, 0}, 500);
        //  registry.add_component(_bulletEntity, RealEngine::Position{position.x, position.y});
        //  registry.add_component(_bulletEntity, RealEngine::Velocity{direction.x * speed,
        //  direction.y * speed, {500.f, 500.f}, 0.f}); registry.add_component(_bulletEntity,
        //  RealEngine::SpriteComponent{_bulletSprite}); registry.add_component(_bulletEntity,
        //  RealEngine::Drawable{}); registry.add_component(_bulletEntity,
        //  RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
        //                                                             "bullet",
        //                                                             false,
        //                                                             RealEngine::CollisionType::OTHER,
        //                                                             [this](RealEngine::CollisionType
        //                                                             collisionType,
        //                                                                    RealEngine::Registry&
        //                                                                    registry,
        //                                                                    RealEngine::Entity
        //                                                                    collider) {
        //                                                                 bullet_collision_handler(collisionType,
        //                                                                 registry, collider);
        //                                                             }});
        //  registry.add_component(_bulletEntity, RealEngine::AutoDestructible{20});
        //  registry.add_component(_bulletEntity, RealEngine::Damage{10});
        std::string message = "Event:New_entity ";
        message += "Type:bullet ";
        message += "Sprite:spaceship_bullet ";
        message += "Position:(" + std::to_string(player.getPosition().x) + "," +
                   std::to_string(player.getPosition().y) + ") ";
        message += "Velocity:(500, 0, {500, 500}, 0) ";
        message += "Collision:(0, 0, 16, 8, bullet, false, OTHER) ";
        message += "AutoDestructible:20 ";
        message += "Damage:10";
        broadCastAll(message);
    }
}