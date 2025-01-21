/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongServer.cpp
*/

#include "PongServer/PongServer.hpp"

#include "GameInstance/PongGameInstance.hpp"

PongServer::PongServer(std::shared_ptr<UDPServer> server, bool server_vision)
    : _server(server),
      _deltaTime(0),
      _deltaTimeBroadcast(0),
      _startTime(std::chrono::steady_clock::now()),
      _gameInstance(std::make_shared<PongGameInstance>(server_vision)) {
    _background = _gameInstance->addAndGetBackground();
    _ball       = _gameInstance->addAndGetBall();
    initCallbacks();
    std::cout << "PongServer started" << std::endl;
}

PongServer::~PongServer() {}

void PongServer::broadcastAllReliable(const std::array<char, 800>& message) {
    for (auto& player : _players) {
        _server->sendReliablePacket(message, player.first);
    }
    for (auto& spectator : _spectators) {
        _server->sendReliablePacket(message, spectator);
    }
}

void PongServer::broadcastAllUnreliable(const std::array<char, 800>& message) {
    for (auto& player : _players) {
        _server->sendUnreliablePacket(message, player.first);
    }
    for (auto& spectator : _spectators) {
        _server->sendUnreliablePacket(message, spectator);
    }
}

void PongServer::run() {
    int server_tick           = 60;
    int server_broadcast_tick = 10;

    while (true) {
        if (_clock.getElapsedTime().asMilliseconds() > 1000 / server_tick) {
            _deltaTime = _clock.restart().asSeconds();

            handleClientMessages();
            _gameInstance->run(_deltaTime);
        }
        if (_broadcastClock.getElapsedTime().asMilliseconds() > 1000 / server_broadcast_tick) {
            _deltaTimeBroadcast = _broadcastClock.restart().asSeconds();
            broadcastStates();
        }
    }
}

void PongServer::handleClientMessages() {
    for (const auto& client : _server->getClients()) {
        auto& player = _players[client];  // WARNING: might have problem if client is not in
                                          // _players
        if (!player) {
            continue;
        }
        const auto& messages = _server->getUnreliableMessagesFromEndpoint(client);

        for (const auto& message : messages) {
            auto baseMessage = PongProtocol::deserialize<800>(message);

            if (baseMessage.messageType == PongProtocol::PLAYER_DIRECTION) {
                handlePlayerDirection(message, client, player);
            }
            if (baseMessage.messageType == PongProtocol::PLAYER_READY) {
                if (std::find(_readyPlayers.begin(), _readyPlayers.end(), baseMessage.uuid) !=
                    _readyPlayers.end()) {
                    continue;
                }
                std::cout << "Player ready, id: " << baseMessage.uuid << std::endl;
                _playerReadyCount++;
                _readyPlayers.push_back(baseMessage.uuid);
                if (_playerReadyCount == 2) {
                    std::cout << "All players ready, launch ball" << std::endl;
                    // _ball get velocity component and set it to 300, 300
                    auto& registry = _gameInstance->getRegistryRef();
                    auto* velocity = registry.getComponent<RealEngine::Velocity>(*_ball);
                    if (velocity) {
                        velocity->vx = 300;
                        velocity->vy = 50;
                    }
                }
            }
        }
    }
}

void PongServer::handlePlayerDirection(const std::array<char, 800>&        message,
                                       const asio::ip::udp::endpoint&      client,
                                       std::shared_ptr<RealEngine::Entity> player) {
    PongProtocol::PlayerDirectionMessage playerDirectionMessage =
        PongProtocol::deserializePlayerDirection<800>(message);
    const auto player_uuid = playerDirectionMessage.uuid;
    const auto timestamp   = playerDirectionMessage.timestamp;

    _playerTimestamps[client] = timestamp;

    // Use consistent server delta time for simulation
    _gameInstance->movePlayer(playerDirectionMessage.direction, _deltaTime, player_uuid);
    _gameInstance->runPlayerSimulation(_players.at(client), _deltaTime);
}

void PongServer::broadcastStates() {
    // Broadcast ball state
    auto& registry     = _gameInstance->getRegistryRef();
    auto* ballPosition = registry.getComponent<RealEngine::Position>(*_ball);
    if (!ballPosition) {
        return;
    }
    PongProtocol::EntityUpdateMessage entityStateMessage = {};
    entityStateMessage.messageType                       = PongProtocol::ENTITY_UPDATE;
    entityStateMessage.uuid                              = *_ball;
    entityStateMessage.x                                 = ballPosition->x;
    entityStateMessage.y                                 = ballPosition->y;
    entityStateMessage.angle                             = -1;
    entityStateMessage.step                              = _deltaTimeBroadcast;
    entityStateMessage.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    std::array<char, 800> serializedMessage = PongProtocol::serialize<800>(entityStateMessage);
    broadcastAllUnreliable(serializedMessage);

    // get netvar container of ball
    auto* netvarContainer = registry.getComponent<RealEngine::NetvarContainer>(*_ball);
    auto* scoreAmount     = registry.getComponent<RealEngine::Score>(*_ball);
    if (!netvarContainer) {
        return;
    }
    int score_to_update = std::any_cast<int>(netvarContainer->netvars["score_to_update"].value);

    // Broadcast player states
    for (const auto& player : _players) {
        if (!player.second) {
            continue;
        }
        auto* position    = registry.getComponent<RealEngine::Position>(*player.second);
        auto* playerScore = registry.getComponent<RealEngine::Score>(*player.second);
        auto* playerNetvarContainer =
            registry.getComponent<RealEngine::NetvarContainer>(*player.second);
        if (playerNetvarContainer &&
            std::any_cast<bool>(playerNetvarContainer->netvars["new_entity"].value)) {
            // send or resend to all to be sure everyone has the new player
            PongProtocol::NewEntityMessage playerMessage;
            playerMessage.messageType = PongProtocol::NEW_ENTITY;
            playerMessage.uuid        = *player.second;
            auto* playerPosition      = registry.getComponent<RealEngine::Position>(player.second);
            auto* playerVelocity      = registry.getComponent<RealEngine::Velocity>(player.second);
            auto* playerInterpolation =
                registry.getComponent<RealEngine::Interpolation>(player.second);
            auto* playerAcceleration =
                registry.getComponent<RealEngine::Acceleration>(player.second);
            auto* playerDrawable = registry.getComponent<RealEngine::Drawable>(player.second);
            if (playerPosition) {
                addComponentToMessage(playerMessage, PongProtocol::ComponentList::POSITION,
                                      *playerPosition);
            }
            if (playerVelocity) {
                addComponentToMessage(playerMessage, PongProtocol::ComponentList::VELOCITY,
                                      *playerVelocity);
            }
            if (playerInterpolation) {
                addComponentToMessage(playerMessage, PongProtocol::ComponentList::INTERPOLATION,
                                      *playerInterpolation);
            }
            if (playerAcceleration) {
                addComponentToMessage(playerMessage, PongProtocol::ComponentList::ACCELERATION,
                                      *playerAcceleration);
            }
            addComponentToMessage(playerMessage, PongProtocol::ComponentList::SPRITE, 3);
            if (playerDrawable) {
                addComponentToMessage(playerMessage, PongProtocol::ComponentList::DRAWABLE,
                                      *playerDrawable);
            }
            // serialize and send player message
            std::array<char, 800> serializedPlayerMessage =
                PongProtocol::serialize<800>(playerMessage);
            broadcastAllReliable(serializedPlayerMessage);
            playerNetvarContainer->netvars["new_entity"].value = false;
        }
        if (playerNetvarContainer &&
            std::any_cast<int>(playerNetvarContainer->netvars["player_index"].value) ==
                score_to_update) {
            playerScore->amount += scoreAmount->amount;
            netvarContainer->netvars["score_to_update"].value = 0;
            // send score update message
            PongProtocol::PlayerUpdateDataMessage playerScoreMessage = {};
            playerScoreMessage.messageType = PongProtocol::PLAYER_UPDATE_DATA;
            playerScoreMessage.uuid        = *player.second;
            playerScoreMessage.score       = playerScore->amount;
            std::array<char, 800> serializedPlayerScoreMessage =
                PongProtocol::serialize<800>(playerScoreMessage);
            broadcastAllReliable(serializedPlayerScoreMessage);
        }
        if (!position) {
            continue;
        }
        PongProtocol::PlayerMoveMessage playerMoveMessage = {};
        playerMoveMessage.messageType                     = PongProtocol::PLAYER_MOVE;
        playerMoveMessage.uuid                            = *player.second;
        playerMoveMessage.x                               = position->x;
        playerMoveMessage.y                               = position->y;
        playerMoveMessage.step                            = _deltaTimeBroadcast;
        playerMoveMessage.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
        std::array<char, 800> serializedMessage = PongProtocol::serialize<800>(playerMoveMessage);
        broadcastAllUnreliable(serializedMessage);
    }
}