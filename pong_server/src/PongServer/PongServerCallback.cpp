/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongServerCallback.cpp
*/

#include "PongServer/PongServer.hpp"

void PongServer::initCallbacks() {
    _server->setNewClientCallback([this](const asio::ip::udp::endpoint& sender) {
        auto registry           = _game_instance->getRegistry();
        bool is_a_player        = false;
        long controlled_players = 0;

        if (_players.size() < 2) {
            is_a_player = true;
        } else {
            _spectators.push_back(sender);
        }

        // send background
        PongProtocol::NewEntityMessage backgroundMessage;
        backgroundMessage.message_type = PongProtocol::NEW_ENTITY;
        backgroundMessage.uuid         = *_background;
        auto* backgroundPosition       = registry->get_component<RealEngine::Position>(_background);
        auto* drawable                 = registry->get_component<RealEngine::Drawable>(_background);
        auto* bgNetvarContainer = registry->get_component<RealEngine::NetvarContainer>(_background);
        auto  bgNetvar          = bgNetvarContainer->getNetvar("sprite_name");
        if (backgroundPosition) {
            addComponentToMessage(backgroundMessage, PongProtocol::ComponentList::POSITION,
                                  *backgroundPosition);
        }
        if (bgNetvar && bgNetvar->value.type() == typeid(std::string)) {
            std::string       sprite_str = std::any_cast<std::string>(bgNetvar->value);
            std::vector<char> spriteData(sprite_str.begin(), sprite_str.end());
            addComponentToMessage(backgroundMessage, PongProtocol::ComponentList::SPRITE,
                                  spriteData);
        }
        if (drawable) {
            addComponentToMessage(backgroundMessage, PongProtocol::ComponentList::DRAWABLE,
                                  *drawable);
        }
        // serialize and send background message
        std::array<char, 800> serializedBackgroundMessage =
            PongProtocol::serialize<800>(backgroundMessage);
        _server->send_reliable_packet(serializedBackgroundMessage, sender);

        // send ball
        PongProtocol::NewEntityMessage ballMessage;
        ballMessage.message_type = PongProtocol::NEW_ENTITY;
        ballMessage.uuid         = *_ball;
        auto* ballPosition       = registry->get_component<RealEngine::Position>(_ball);
        auto* ballVelocity       = registry->get_component<RealEngine::Velocity>(_ball);
        auto* ballInterpolation  = registry->get_component<RealEngine::Interpolation>(_ball);
        auto* ballNetvarContainer    = registry->get_component<RealEngine::NetvarContainer>(_ball);
        auto  ballNetvar             = ballNetvarContainer->getNetvar("sprite_name");
        // auto* ballAcceleration   = registry->get_component<RealEngine::Acceleration>(_ball);
        auto* ballDrawable = registry->get_component<RealEngine::Drawable>(_ball);
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
        if (ballNetvar && ballNetvar->value.type() == typeid(std::string)) {
            std::string       sprite_str = std::any_cast<std::string>(ballNetvar->value);
            std::vector<char> spriteData(sprite_str.begin(), sprite_str.end());
            addComponentToMessage(ballMessage, PongProtocol::ComponentList::SPRITE, spriteData);
        }
        // std::string       ballSprite_str = "ball";
        // std::vector<char> ballSpriteData(ballSprite_str.begin(), ballSprite_str.end());
        // addComponentToMessage(ballMessage, PongProtocol::ComponentList::SPRITE, ballSpriteData);
        if (ballDrawable) {
            addComponentToMessage(ballMessage, PongProtocol::ComponentList::DRAWABLE,
                                  *ballDrawable);
        }
        // serialize and send ball message
        std::array<char, 800> serializedBallMessage = PongProtocol::serialize<800>(ballMessage);
        _server->send_reliable_packet(serializedBallMessage, sender);

        if (is_a_player) {
            if (_players.size() == 1) {
                _players[sender] = _game_instance->addAndGetPlayer({400, 550}, 2);
                _playerTimestamps[sender] =
                    std::chrono::system_clock::now().time_since_epoch().count();
            }
            if (_players.size() == 0) {
                _players[sender] = _game_instance->addAndGetPlayer({400, 50}, 1);
                _playerTimestamps[sender] =
                    std::chrono::system_clock::now().time_since_epoch().count();
            }
            controlled_players = *_players[sender];
        }

        // send players
        for (auto& player : _players) {
            PongProtocol::NewEntityMessage playerMessage;
            playerMessage.message_type = PongProtocol::NEW_ENTITY;
            playerMessage.uuid         = *player.second;
            auto* playerPosition = registry->get_component<RealEngine::Position>(player.second);
            auto* playerVelocity = registry->get_component<RealEngine::Velocity>(player.second);
            auto* playerNetvarContainer =
                registry->get_component<RealEngine::NetvarContainer>(player.second);
            auto  playerNetvar = playerNetvarContainer->getNetvar("sprite_name");
            auto* playerInterpolation =
                registry->get_component<RealEngine::Interpolation>(player.second);
            // auto* playerAcceleration   =
            // registry->get_component<RealEngine::Acceleration>(player.second);
            auto* playerDrawable = registry->get_component<RealEngine::Drawable>(player.second);
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
            // if (playerAcceleration) {
            //     addComponentToMessage(playerMessage, PongProtocol::ComponentList::ACCELERATION,
            //                           *playerAcceleration);
            // }
            if (playerNetvar && playerNetvar->value.type() == typeid(std::string)) {
                std::string       sprite_str = std::any_cast<std::string>(playerNetvar->value);
                std::cout << "sprite_str: " << sprite_str << std::endl;
                std::vector<char> spriteData(sprite_str.begin(), sprite_str.end());
                addComponentToMessage(playerMessage, PongProtocol::ComponentList::SPRITE,
                                      spriteData);
            }
            // std::string       playerSprite_str = "player_bar";
            // std::vector<char> playerSpriteData(playerSprite_str.begin(), playerSprite_str.end());
            // addComponentToMessage(playerMessage, PongProtocol::ComponentList::SPRITE,
            //                       playerSpriteData);
            if (playerDrawable) {
                addComponentToMessage(playerMessage, PongProtocol::ComponentList::DRAWABLE,
                                      *playerDrawable);
            }
            addComponentToMessage(playerMessage, PongProtocol::ComponentList::CONTROLABLE,
                                  RealEngine::Controllable{});
            // serialize and send player message
            std::array<char, 800> serializedPlayerMessage =
                PongProtocol::serialize<800>(playerMessage);
            _server->send_reliable_packet(serializedPlayerMessage, sender);

            // send player score
            PongProtocol::PlayerUpdateDataMessage playerScoreMessage;
            playerScoreMessage.message_type = PongProtocol::PLAYER_UPDATE_DATA;
            playerScoreMessage.uuid         = *player.second;
            auto* playerScore = registry->get_component<RealEngine::Score>(player.second);
            if (playerScore) {
                playerScoreMessage.score = playerScore->amount;
            }
            // serialize and send player score message
            std::array<char, 800> serializedPlayerScoreMessage =
                PongProtocol::serialize<800>(playerScoreMessage);
            _server->send_reliable_packet(serializedPlayerScoreMessage, sender);
        }

        // send controlled player
        if (is_a_player) {
            PongProtocol::BaseMessage playerControlMessage;
            playerControlMessage.message_type = PongProtocol::PLAYER_CONTROL;
            playerControlMessage.uuid         = controlled_players;
            // serialize and send controlled player message
            std::array<char, 800> serializedPlayerControlMessage =
                PongProtocol::serialize<800>(playerControlMessage);
            _server->send_reliable_packet(serializedPlayerControlMessage, sender);
        }
    });
}
