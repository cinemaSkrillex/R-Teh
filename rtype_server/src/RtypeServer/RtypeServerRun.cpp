/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerRun
*/

#include "../../include/RtypeServer/RtypeServer.hpp"
#include "../../include/shared/RtypeServerProtocol.hpp"

void RtypeServer::run() {
    auto        log                   = std::make_shared<Log>("RtypeServer.log");
    int         server_tick           = _server_config.getConfigItem<int>("SERVER_TICK");
    int         server_broadcast_tick = _server_config.getConfigItem<int>("SERVER_BROADCAST_TICK");
    const float fixedTimeStep   = 1.0f / server_tick;  // MAP fixed time-step (client has the same)
    float       accumulatedTime = 0.0f;

    while (true) {
        if (_clock.getElapsedTime().asMilliseconds() > 1000 / server_tick) {
            // Reset the clock for the next tick
            _deltaTime = _clock.restart().asSeconds();
            accumulatedTime += _deltaTime;

            handleClientMessages();
            runGameInstance(_deltaTime);

            while (accumulatedTime >= fixedTimeStep) {
                updateMapState(fixedTimeStep);
                accumulatedTime -= fixedTimeStep;
            }
        }
        if (_broadcastClock.getElapsedTime().asMilliseconds() > 1000 / server_broadcast_tick) {
            _deltaTimeBroadcast = _broadcastClock.restart().asSeconds();
            broadcastStates();
        }
    }
}

void RtypeServer::handleClientMessages() {
    for (const auto& client : _server->getClients()) {
        auto&       player   = _players.at(client);
        const auto& messages = _server->get_unreliable_messages_from_endpoint(client);

        for (const auto& message : messages) {
            auto baseMessage = RTypeProtocol::deserialize<800>(message);

            if (baseMessage.message_type == RTypeProtocol::PLAYER_DIRECTION) {
                runSimulation(message, client, player);
            } else {
                runEvent(message, client, player);
            }
        }
    }
}

void RtypeServer::initEventHandlers() {
    eventHandlers[RTypeProtocol::MessageType::SHOOT_EVENT] = std::make_unique<ShootEvent>();
    eventHandlers[RTypeProtocol::MessageType::HOLD_SHOOT_EVENT] =
        std::make_unique<HoldShootEvent>();
    eventHandlers[RTypeProtocol::MessageType::RELEASE_SHOOT_EVENT] =
        std::make_unique<ReleaseShootEvent>();
}

void RtypeServer::runGameInstance(float deltaTime) {
    auto destroyedEntities = _game_instance->run(_deltaTime);
    if (!destroyedEntities.empty()) {
        RTypeProtocol::DestroyEntityMessage destroyMessage;
        destroyMessage.message_type = RTypeProtocol::MessageType::DESTROY_ENTITY;
        destroyMessage.uuid         = 0;
        destroyMessage.entity_ids.reserve(destroyedEntities.size());
        destroyMessage.entity_ids.insert(destroyMessage.entity_ids.end(), destroyedEntities.begin(),
                                         destroyedEntities.end());
        std::array<char, 800> serializedDestroyMessage =
            RTypeProtocol::serialize<800>(destroyMessage);
        broadcastAllReliable(serializedDestroyMessage);
    }
}

void RtypeServer::broadcastStates() {
    // Broadcast the state of all players
    for (const auto& player : _players) {
        broadcastPlayerState(player.second);
    }
    // Broadcast entity states
    // TODO : Implement entity state broadcasting
}