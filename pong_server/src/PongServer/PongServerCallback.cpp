/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongServerCallback.cpp
*/

#include "PongServer/PongServer.hpp"

void PongServer::initCallbacks() {
    _server->setNewClientCallback([this](const asio::ip::udp::endpoint& sender) {
        auto registry           = _gameInstance->getRegistry();
        bool is_a_player        = false;
        long controlled_players = 0;

        if (_players.size() < 2) {
            is_a_player = true;
        } else {
            _spectators.push_back(sender);
        }

        // send background
        PongProtocol::NewEntityMessage backgroundMessage;
        backgroundMessage.messageType = PongProtocol::NEW_ENTITY;
        backgroundMessage.uuid        = *_background;
        auto* backgroundPosition      = registry->getComponent<RealEngine::Position>(_background);
        auto* drawable                = registry->getComponent<RealEngine::Drawable>(_background);
        if (backgroundPosition) {
            addComponentToMessage(backgroundMessage, PongProtocol::ComponentList::POSITION,
                                  *backgroundPosition);
        }
        addComponentToMessage(backgroundMessage, PongProtocol::ComponentList::SPRITE, 1);
        if (drawable) {
            addComponentToMessage(backgroundMessage, PongProtocol::ComponentList::DRAWABLE,
                                  *drawable);
        }
        // serialize and send background message
        std::array<char, 800> serializedBackgroundMessage =
            PongProtocol::serialize<800>(backgroundMessage);
        _server->sendReliablePacket(serializedBackgroundMessage, sender);

        // send ball
        PongProtocol::NewEntityMessage ballMessage;
        ballMessage.messageType = PongProtocol::NEW_ENTITY;
        ballMessage.uuid        = *_ball;
        auto* ballPosition      = registry->getComponent<RealEngine::Position>(_ball);
        auto* ballVelocity      = registry->getComponent<RealEngine::Velocity>(_ball);
        auto* ballInterpolation = registry->getComponent<RealEngine::Interpolation>(_ball);
        // auto* ballAcceleration   = registry->getComponent<RealEngine::Acceleration>(_ball);
        auto* ballDrawable = registry->getComponent<RealEngine::Drawable>(_ball);
        if (ballPosition) {
            addComponentToMessage(ballMessage, PongProtocol::ComponentList::POSITION,
                                  *ballPosition);
        }
        if (ballVelocity) {
            addComponentToMessage(ballMessage, PongProtocol::ComponentList::VELOCITY,
                                  *ballVelocity);
        }
        if (ballInterpolation) {
            addComponentToMessage(ballMessage, PongProtocol::ComponentList::INTERPOLATION,
                                  *ballInterpolation);
        }
        // if (ballAcceleration) {
        //     addComponentToMessage(ballMessage, PongProtocol::ComponentList::ACCELERATION,
        //                           *ballAcceleration);
        // }
        addComponentToMessage(ballMessage, PongProtocol::ComponentList::SPRITE, 2);
        // std::string       ballSprite_str = "ball";
        // std::vector<char> ballSpriteData(ballSprite_str.begin(), ballSprite_str.end());
        // addComponentToMessage(ballMessage, PongProtocol::ComponentList::SPRITE, ballSpriteData);
        if (ballDrawable) {
            addComponentToMessage(ballMessage, PongProtocol::ComponentList::DRAWABLE,
                                  *ballDrawable);
        }
        // serialize and send ball message
        std::array<char, 800> serializedBallMessage = PongProtocol::serialize<800>(ballMessage);
        _server->sendReliablePacket(serializedBallMessage, sender);

        if (is_a_player) {
            if (_players.size() == 1) {
                _players[sender] = _gameInstance->addAndGetPlayer({20, 500}, 2);
                _playerTimestamps[sender] =
                    std::chrono::system_clock::now().time_since_epoch().count();
            }
            if (_players.size() == 0) {
                _players[sender] = _gameInstance->addAndGetPlayer({780, 500}, 1);
                _playerTimestamps[sender] =
                    std::chrono::system_clock::now().time_since_epoch().count();
            }
            controlled_players = *_players[sender];
        }

        // send players
        for (auto& player : _players) {
            PongProtocol::NewEntityMessage playerMessage;
            playerMessage.messageType = PongProtocol::NEW_ENTITY;
            playerMessage.uuid        = *player.second;
            auto* playerPosition      = registry->getComponent<RealEngine::Position>(player.second);
            auto* playerVelocity      = registry->getComponent<RealEngine::Velocity>(player.second);
            auto* playerInterpolation =
                registry->getComponent<RealEngine::Interpolation>(player.second);
            auto* playerAcceleration =
                registry->getComponent<RealEngine::Acceleration>(player.second);
            auto* playerDrawable = registry->getComponent<RealEngine::Drawable>(player.second);
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
            // std::string       playerSprite_str = "player_bar";
            // std::vector<char> playerSpriteData(playerSprite_str.begin(), playerSprite_str.end());
            // addComponentToMessage(playerMessage, PongProtocol::ComponentList::SPRITE,
            //                       playerSpriteData);
            if (playerDrawable) {
                addComponentToMessage(playerMessage, PongProtocol::ComponentList::DRAWABLE,
                                      *playerDrawable);
            }
            // serialize and send player message
            std::array<char, 800> serializedPlayerMessage =
                PongProtocol::serialize<800>(playerMessage);
            _server->sendReliablePacket(serializedPlayerMessage, sender);

            // send player score
            PongProtocol::PlayerUpdateDataMessage playerScoreMessage;
            playerScoreMessage.messageType = PongProtocol::PLAYER_UPDATE_DATA;
            playerScoreMessage.uuid        = *player.second;
            auto* playerScore = registry->getComponent<RealEngine::Score>(player.second);
            if (playerScore) {
                playerScoreMessage.score = playerScore->amount;
            }
            // serialize and send player score message
            std::array<char, 800> serializedPlayerScoreMessage =
                PongProtocol::serialize<800>(playerScoreMessage);
            _server->sendReliablePacket(serializedPlayerScoreMessage, sender);
        }

        // send controlled player
        if (is_a_player) {
            PongProtocol::BaseMessage playerControlMessage;
            playerControlMessage.messageType = PongProtocol::PLAYER_CONTROL;
            playerControlMessage.uuid        = controlled_players;
            // serialize and send controlled player message
            std::array<char, 800> serializedPlayerControlMessage =
                PongProtocol::serialize<800>(playerControlMessage);
            _server->sendReliablePacket(serializedPlayerControlMessage, sender);
        }
    });
}
