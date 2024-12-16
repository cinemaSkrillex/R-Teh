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
        // check player entity netvar shootCooldown
        // auto* component = registry.get_component<Component>(entity);
        auto* netvar =
            _game_instance->getRegistry()->get_component<RealEngine::Netvar>(player.getEntity());
        if (netvar == nullptr) {
            return;
        }
        if (std::any_cast<float>(netvar->value) > 0) {
            return;
        }
        netvar->value = 0.5f;

        const sf::Vector2f bullet_position = player.getPosition() + sf::Vector2f(32.5f, 7.5f);

        // _game_instance->addAndGetBullet(bullet_position, {1, 0}, 500);
        auto bullet = _game_instance->addAndGetBullet(bullet_position, {1, 0}, 500);
        // WARNING: this used fixed values and need to be updated if updates are made to bullet
        // creation. Should store returned addAndGetBullet and use it's value.
        std::string message = "Event:New_entity ";
        message += "Uuid:" + std::to_string(*bullet) + " ";
        message += "Type:bullet ";
        message += "Sprite:bullet ";
        message += "Position:(" + std::to_string(bullet_position.x) + "," +
                   std::to_string(bullet_position.y) + ") ";
        message += "Velocity:(500,0,{500,500},0) ";
        message += "Collision:(0,0,16,8,bullet,false,OTHER) ";
        message += "AutoDestructible:5 ";
        message += "Drawable:true ";
        broadCastAll(message);
    }
}