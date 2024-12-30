/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerCallbacks
*/

#include "../../include/RtypeServer.hpp"

std::vector<int> deserialize_int_vector(const std::vector<char>& buffer) {
    std::vector<int> int_vector;
    int_vector.reserve(buffer.size() / sizeof(int));

    for (size_t i = 0; i < buffer.size(); i += sizeof(int)) {
        int value;
        std::memcpy(&value, buffer.data() + i, sizeof(int));
        int_vector.push_back(value);
    }

    return int_vector;
}

void print_raw_data(const std::vector<char>& data) {
    std::cout << "Raw data: ";
    for (char byte : data) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;
}

// std::cout << "New client connected: " << sender << std::endl;

// auto charvector = _server->get_last_unreliable_packet_data();

// // Print raw data received
// print_raw_data(charvector);

// auto int_vector = deserialize_int_vector(charvector);

// std::cout << "Data: ";
// for (int value : int_vector) {
//     std::cout << value << " ";
// }
// std::cout << std::endl;

void RtypeServer::initCallbacks() {
    _server->setNewClientCallback([this](const asio::ip::udp::endpoint& sender) {
        std::cout << "New client connected: " << sender << std::endl;
        sf::Vector2f player_start_position =
            _server_config.getConfigItem<sf::Vector2f>("PLAYER_START_POSITION");
        // create Player entity
        long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now() - _startTime)
                                .count();
        auto playerEntity = _game_instance->addAndGetPlayer(player_start_position);
        auto player =
            Player(*playerEntity, elapsed_time, playerEntity, _game_instance->getRegistry());
        // Notify all other clients about the new client
        for (const auto& client : _server->getClients()) {
            std::cout << "Client: " << client << std::endl;
            if (client != sender) {
                // const std::string message =
                //     "Event:New_client Uuid:" + std::to_string(*playerEntity) + " Position:(" +
                //     std::to_string(player_start_position.x) + "," +
                //     std::to_string(player_start_position.y) + ")";
                // std::cout << "Sending message: " << message << std::endl;

                RTypeProtocol::PlayerMoveMessage newClientMessage;
                newClientMessage.message_type = RTypeProtocol::NEW_CLIENT;
                newClientMessage.uuid         = *playerEntity;
                long player_entity_uuid       = *playerEntity;
                std::cout << "Player entity uuid: " << player_entity_uuid << std::endl;
                newClientMessage.x = player_start_position.x;
                std::cout << "Player start position x: " << player_start_position.x << std::endl;
                newClientMessage.y = player_start_position.y;
                std::cout << "Player start position y: " << player_start_position.y << std::endl;
                newClientMessage.timestamp = elapsed_time;
                std::cout << "Elapsed time: " << elapsed_time << std::endl;
                std::cout << "Deserialized new client message:" << std::endl;
                std::array<char, 1024> serializedMessage = RTypeProtocol::serialize<1024>(
                    newClientMessage);  // needs to be changed to TEMPLATE BUFFER SIZE
                                        // Print the serialized data (hexadecimal representation)

                // directly deserialize the message
                // RTypeProtocol::BaseMessage baseMessage =
                //     RTypeProtocol::deserialize<1024>(serializedMessage);
                // std::cout << "Deserialized BaseMessage:" << std::endl;
                // std::cout << "  Message Type: " << baseMessage.message_type << std::endl;

                // std::cout << "Serialized data: ";
                // for (const auto& byte : serializedMessage) {
                //     std::cout << std::hex << (int)byte << " ";  // Print each byte in hexadecimal
                // }
                // std::cout << std::dec
                //           << std::endl;  // Reset back to decimal formatting for subsequent
                //           outputs
                std::cout << "Serialized message size: " << serializedMessage.size() << " bytes"
                          << std::endl;

                _server->send_reliable_packet(serializedMessage, client);

                // _server->send_reliable_packet(message, client);
            }
        }
        // Create the uuid for each new client
        std::string message = "Event:Synchronize Uuid:" + std::to_string(*playerEntity) +
                              " Clock:" + formatTimestamp(_startTime) + " Position:(" +
                              std::to_string(player_start_position.x) + "," +
                              std::to_string(player_start_position.y) + ") Players:[";
        bool first = true;
        for (const auto& player_pair : _players) {
            if (!first) message += "|";
            first              = false;
            const auto& player = player_pair.second;
            message += std::to_string(player.getUUID()) + ",(" +
                       std::to_string(player.getPosition().x) + "," +
                       std::to_string(player.getPosition().y) + ")";
        }
        message += "]";
        // _server->send_reliable_packet(message, sender);

        // Send all the entities to the new client, so it can synchronize and move
        for (const auto& mob : _game_instance->getSimpleMobs()) {
            if (!mob) continue;
            auto* position =
                _game_instance->getRegistry()->get_component<RealEngine::Position>(mob);
            if (!position) continue;
            auto* destructible =
                _game_instance->getRegistry()->get_component<RealEngine::AutoDestructible>(mob);
            if (!destructible) continue;
            auto* velocity =
                _game_instance->getRegistry()->get_component<RealEngine::Velocity>(mob);
            if (!velocity) continue;
            auto* rotation =
                _game_instance->getRegistry()->get_component<RealEngine::Rotation>(mob);

            std::string velocityStr =
                std::to_string(velocity->vx) + "," + std::to_string(velocity->vy) + ",{" +
                std::to_string(velocity->maxSpeed.x) + "," + std::to_string(velocity->maxSpeed.y) +
                "}," + std::to_string(velocity->airFrictionForce);
            std::string MobMessage = "Event:New_entity";
            MobMessage += " Type:mob";
            MobMessage += " Uuid:" + std::to_string(*mob);
            MobMessage += " Sprite:enemy";
            MobMessage += " Position:(" + std::to_string(position->x) + "," +
                          std::to_string(position->y) + ")";
            MobMessage += " Velocity:(" + velocityStr + ")";
            MobMessage += " Collision:(0,0,16,8,mob,false,OTHER)";
            MobMessage += " AutoDestructible:" + std::to_string(destructible->lifeTime);
            if (rotation) MobMessage += " Rotation:" + std::to_string(rotation->angle);
            MobMessage += " Drawable:true";

            // _server->send_reliable_packet(MobMessage, sender);
        }
        _players[sender] = player;
    });
}