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

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> createSynchronizeMessage(
    long uuid, long timestamp, float x, float y,
    const std::vector<std::pair<long, sf::Vector2f>>& activePlayerUUIDs) {
    // Populate the SynchronizeMessage structure
    RTypeProtocol::SynchronizeMessage syncMessage;
    syncMessage.message_type = RTypeProtocol::SYNCHRONIZE;  // Message type
    syncMessage.uuid         = uuid;                        // Player UUID
    syncMessage.timestamp    = timestamp;                   // Synchronization timestamp
    syncMessage.x            = x;                           // Player X position
    syncMessage.y            = y;                           // Player Y position
    syncMessage.players      = activePlayerUUIDs;           // List of active player UUIDs

    // Serialize the message
    return RTypeProtocol::serialize<BUFFER_SIZE>(syncMessage);
}

void RtypeServer::initCallbacks() {
    _server->setNewClientCallback([this](const asio::ip::udp::endpoint& sender) {
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
            if (client != sender) {
                RTypeProtocol::PlayerMoveMessage newClientMessage = {};
                newClientMessage.message_type                     = RTypeProtocol::NEW_CLIENT;
                newClientMessage.uuid                             = *playerEntity;
                long player_entity_uuid                           = *playerEntity;
                newClientMessage.x                                = player_start_position.x;
                newClientMessage.y                                = player_start_position.y;
                newClientMessage.timestamp                        = elapsed_time;
                std::array<char, 800> serializedMessage           = RTypeProtocol::serialize<800>(
                    newClientMessage);  // needs to be changed to TEMPLATE BUFFER SIZE
                _server->send_reliable_packet(serializedMessage, client);
            }
        }
        // Create the uuid for each new client
        std::string message = "Event:Synchronize Uuid:" + std::to_string(*playerEntity) +
                              " Clock:" + formatTimestamp(_startTime) + " Position:(" +
                              std::to_string(player_start_position.x) + "," +
                              std::to_string(player_start_position.y) + ") Players:[";
        bool                                       first = true;
        std::vector<std::pair<long, sf::Vector2f>> activePlayerUUIDs;
        for (const auto& player_pair : _players) {
            if (!first) message += "|";
            first              = false;
            const auto& player = player_pair.second;
            message += std::to_string(player.getUUID()) + ",(" +
                       std::to_string(player.getPosition().x) + "," +
                       std::to_string(player.getPosition().y) + ")";
            activePlayerUUIDs.push_back({player.getUUID(), player.getPosition()});
        }
        message += "]";
        std::array<char, 800> synchronizeMessage = createSynchronizeMessage<800>(
            *playerEntity,
            std::chrono::duration_cast<std::chrono::milliseconds>(_startTime.time_since_epoch())
                .count(),
            player_start_position.x, player_start_position.y, activePlayerUUIDs);

        _server->send_reliable_packet(synchronizeMessage, sender);

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

            // _server->send_reliable_packet(MobMessage, sender);
            RTypeProtocol::NewEntityMessage eventMessage;
            eventMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
            eventMessage.uuid         = *mob;
            // eventMessage.event_type   = RTypeProtocol::EventType::SHOOT; //TODO change to NEW_ENTITY after rework

            // Serialize position component
            std::vector<char> positionData(sizeof(sf::Vector2f));
            std::memcpy(positionData.data(), position, sizeof(sf::Vector2f));
            eventMessage.components.push_back({RTypeProtocol::ComponentList::POSITION, positionData});

            // Serialize velocity component
            std::vector<char> velocityData(sizeof(RealEngine::Velocity));
            std::memcpy(velocityData.data(), velocity, sizeof(RealEngine::Velocity));
            eventMessage.components.push_back({RTypeProtocol::ComponentList::VELOCITY, velocityData});

            // Serialize rotation component
            if (rotation) {
                std::vector<char> rotationData(sizeof(RealEngine::Rotation));
                std::memcpy(rotationData.data(), rotation, sizeof(RealEngine::Rotation));
                eventMessage.components.push_back({RTypeProtocol::ComponentList::ROTATION, rotationData});
            }

            // Serialize collision component
            sf::FloatRect             bounds      = {0, 0, 16, 8};
            std::string               id          = "mob";
            bool                      isColliding = false;
            RealEngine::CollisionType type        = RealEngine::CollisionType::OTHER;

            std::vector<char> collisionData(sizeof(bounds) + id.size() + 1 + sizeof(isColliding) +
                                            sizeof(type));
            char*             collisionPtr = collisionData.data();
            std::memcpy(collisionPtr, &bounds, sizeof(bounds));
            collisionPtr += sizeof(bounds);
            std::memcpy(collisionPtr, id.c_str(), id.size() + 1);
            collisionPtr += id.size() + 1;
            std::memcpy(collisionPtr, &isColliding, sizeof(isColliding));
            collisionPtr += sizeof(isColliding);
            std::memcpy(collisionPtr, &type, sizeof(type));
            eventMessage.components.push_back({RTypeProtocol::ComponentList::COLLISION, collisionData});

            // Serialize auto destructible component
            float             autoDestructible = destructible->lifeTime;
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
            std::string       sprite = "enemy";
            std::vector<char> spriteData(sprite.begin(), sprite.end());
            eventMessage.components.push_back({RTypeProtocol::ComponentList::SPRITE, spriteData});

            std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(eventMessage);
            _server->send_reliable_packet(serializedEventMessage, sender);
        }
        _players[sender] = player;
    });
}
