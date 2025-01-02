/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeEvent
*/

#include "../../include/RtypeServer.hpp"

// void RtypeServer::runEvent(const std::unordered_map<std::string, std::string>& parsed_data,
//                            asio::ip::udp::endpoint& client, Player& player) {
//     if (parsed_data.at("Event") == "Shoot") {
//         // check player entity netvar shootCooldown
//         // auto* component = registry.get_component<Component>(entity);
//         auto* netvar =
//             _game_instance->getRegistry()->get_component<RealEngine::Netvar>(player.getEntity());
//         if (netvar == nullptr) {
//             return;
//         }
//         if (std::any_cast<float>(netvar->value) > 0) {
//             return;
//         }
//         netvar->value = 0.5f;

//         const sf::Vector2f bullet_position = player.getPosition() + sf::Vector2f(32.5f, 7.5f);

//         // _game_instance->addAndGetBullet(bullet_position, {1, 0}, 500);
//         auto bullet = _game_instance->addAndGetBullet(bullet_position, {1, 0}, 500);
//         // WARNING: this used fixed values and need to be updated if updates are made to bullet
//         // creation. Should store returned addAndGetBullet and use it's value.
//         std::string message = "Event:New_entity ";
//         message += "Uuid:" + std::to_string(*bullet) + " ";
//         message += "Type:bullet ";
//         message += "Sprite:bullet ";
//         message += "Position:(" + std::to_string(bullet_position.x) + "," +
//                    std::to_string(bullet_position.y) + ") ";
//         message += "Velocity:(500,0,{500,500},0) ";
//         message += "Collision:(0,0,16,8,bullet,false,OTHER) ";
//         message += "AutoDestructible:5 ";
//         message += "Drawable:true ";
//         broadCastAll(message);
//     }
// }

void RtypeServer::runEvent(const std::array<char, 800>& buffer, asio::ip::udp::endpoint& client,
                           Player& player) {
    RTypeProtocol::EventMessage eventMessage = RTypeProtocol::deserializeEventMessage(buffer);

    if (eventMessage.event_type == RTypeProtocol::EventType::SHOOT) {
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
        // std::string message = "Event:New_entity ";
        // message += "Uuid:" + std::to_string(*bullet) + " ";
        // message += "Type:bullet ";
        // message += "Sprite:bullet ";
        // message += "Position:(" + std::to_string(bullet_position.x) + "," +
        //            std::to_string(bullet_position.y) + ") ";
        // message += "Velocity:(500,0,{500,500},0) ";
        // message += "Collision:(0,0,16,8,bullet,false,OTHER) ";
        // message += "AutoDestructible:5 ";
        // message += "Drawable:true ";
        // broadCastAll(message);
        eventMessage.message_type = RTypeProtocol::MessageType::EVENT_MESSAGE;
        eventMessage.uuid         = *bullet;
        eventMessage.event_type   = RTypeProtocol::EventType::SHOOT;

        // Serialize position component
        std::vector<char> positionData(sizeof(sf::Vector2f));
        std::memcpy(positionData.data(), &bullet_position, sizeof(sf::Vector2f));
        eventMessage.components.push_back({RTypeProtocol::ComponentList::POSITION, positionData});

        // Serialize velocity component
        sf::Vector2f      velocity = {500.f, 0.f};
        std::vector<char> velocityData(sizeof(sf::Vector2f));
        std::memcpy(velocityData.data(), &velocity, sizeof(sf::Vector2f));
        eventMessage.components.push_back({RTypeProtocol::ComponentList::VELOCITY, velocityData});

        // Serialize collision component
        sf::IntRect       collision = {0, 0, 16, 8};
        std::vector<char> collisionData(sizeof(sf::IntRect));
        std::memcpy(collisionData.data(), &collision, sizeof(sf::IntRect));
        eventMessage.components.push_back({RTypeProtocol::ComponentList::COLLISION, collisionData});

        // Serialize auto destructible component
        int               autoDestructible = 5;
        std::vector<char> autoDestructibleData(sizeof(int));
        std::memcpy(autoDestructibleData.data(), &autoDestructible, sizeof(int));
        eventMessage.components.push_back(
            {RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE, autoDestructibleData});

        // Serialize drawable component
        bool              drawable = true;
        std::vector<char> drawableData(sizeof(bool));
        std::memcpy(drawableData.data(), &drawable, sizeof(bool));
        eventMessage.components.push_back({RTypeProtocol::ComponentList::DRAWABLE, drawableData});

        // Serialize sprite component
        std::string       sprite = "bullet";
        std::vector<char> spriteData(sprite.begin(), sprite.end());
        eventMessage.components.push_back({RTypeProtocol::ComponentList::SPRITE, spriteData});

        std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(eventMessage);
        // broadcastAll(serializedEventMessage);
        RTypeProtocol::EventMessage deserializedEventMessage =
            RTypeProtocol::deserializeEventMessage(serializedEventMessage);

        for (const auto& component : deserializedEventMessage.components) {
            switch (component.first) {
                case RTypeProtocol::ComponentList::POSITION: {
                    sf::Vector2f position;
                    std::memcpy(&position, component.second.data(), sizeof(position));
                    std::cout << "Position: (" << position.x << ", " << position.y << ")\n";
                    break;
                }
                case RTypeProtocol::ComponentList::VELOCITY: {
                    sf::Vector2f velocity;
                    std::memcpy(&velocity, component.second.data(), sizeof(velocity));
                    std::cout << "Velocity: (" << velocity.x << ", " << velocity.y << ")\n";
                    break;
                }
                case RTypeProtocol::ComponentList::COLLISION: {
                    sf::IntRect collision;
                    std::memcpy(&collision, component.second.data(), sizeof(collision));
                    std::cout << "Collision: (" << collision.left << ", " << collision.top << ", "
                              << collision.width << ", " << collision.height << ")\n";
                    break;
                }
                case RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE: {
                    int autoDestructible;
                    std::memcpy(&autoDestructible, component.second.data(),
                                sizeof(autoDestructible));
                    std::cout << "AutoDestructible: " << autoDestructible << "\n";
                    break;
                }
                case RTypeProtocol::ComponentList::DRAWABLE: {
                    bool drawable;
                    std::memcpy(&drawable, component.second.data(), sizeof(drawable));
                    std::cout << "Drawable: " << std::boolalpha << drawable << "\n";
                    break;
                }
                case RTypeProtocol::ComponentList::SPRITE: {
                    std::string sprite(component.second.begin(), component.second.end());
                    std::cout << "Sprite: " << sprite << "\n";
                    break;
                }
                default:
                    std::cout << "Unknown component type: " << component.first << "\n";
                    break;
            }
        }

        // eventMessage.components.push_back
    }
}