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

            // while (accumulatedTime >= fixedTimeStep) {
            //     updateMapState(fixedTimeStep);
            //     accumulatedTime -= fixedTimeStep;
            // }
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

    // Broadcast new entities
    // Broadcast the state of all players and entities

    //Get the entities that have the netvar new_entity set to true
    auto entities = _game_instance->getRegistry()->view<RealEngine::NetvarContainer>();

    for (auto entity : entities) {
        auto* netvarContainer = _game_instance->getRegistry()->get_component<RealEngine::NetvarContainer>(entity);
        if (!netvarContainer) continue;
        auto newEntity = netvarContainer->netvars.find("new_entity");
        if (newEntity != netvarContainer->netvars.end()) {
            auto* newEntityValue = std::any_cast<bool>(&newEntity->second.value);
            if (newEntityValue && *newEntityValue) {
                std::cout << "New entity detected" << std::endl;
                // auto* spriteName = std::any_cast<std::string>(&netvarContainer->netvars.find("sprite_name")->second.value);
                // if (spriteName) {
                //     RTypeProtocol::NewEntityMessage newEntityMessage;
                //     newEntityMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
                //     newEntityMessage.uuid         = 0;
                //     newEntityMessage.entity_id    = entity;
                //     newEntityMessage.sprite_name  = *spriteName;
                //     std::array<char, 800> serializedNewEntityMessage =
                //         RTypeProtocol::serialize<800>(newEntityMessage);
                //     broadcastAllReliable(serializedNewEntityMessage);
                // }
            }
        }
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