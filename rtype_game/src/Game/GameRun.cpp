/*
** EPITECH PROJECT, 2024
**
** File description:
** GameRun
*/

#include "Game/Game.hpp"

void rtype::Game::run() {
    while (_window.isOpen()) {
        if (_clock.getElapsedTime().asMilliseconds() <= 1000 / 60) continue;

        _deltaTime = _clock.restart().asSeconds();
        _window.update();
        _window.clear();
        handleSignal(_clientUDP->get_last_reliable_packet_data());
        handleSignal(_clientUDP->get_last_unreliable_packet_data());
        _registry.run_systems(_deltaTime);
        const sf::IntRect direction = getPlayerNormalizedDirection();
        _window.display();
        auto client_now = std::chrono::steady_clock::now();
        long client_elapsed_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(client_now - _startTime).count();
        long delta_time = client_elapsed_time - _serverTime;
        // Create a PlayerDirectionMessage
        RTypeProtocol::PlayerDirectionMessage playerDirectionMessage;
        playerDirectionMessage.message_type = RTypeProtocol::PLAYER_DIRECTION;
        playerDirectionMessage.uuid         = _localPlayerUUID;
        playerDirectionMessage.direction    = direction;
        playerDirectionMessage.timestamp    = delta_time;

        // Serialize the PlayerDirectionMessage
        std::array<char, 800> serializedPlayerDirectionMessage =
            RTypeProtocol::serialize<800>(playerDirectionMessage);

        _clientUDP->send_unreliable_packet(serializedPlayerDirectionMessage);
        updateMap(_deltaTime);
    }
    exit(0);
}

void rtype::Game::updateMap(float deltaTime) {
    const float fixedTimeStep = 1.0f / _serverTick;
    if (!_isMapLoaded) {
        return;
    }
    _ClientX_level_position += _ClientScrollingSpeed * fixedTimeStep;

    auto blockTagEntities = _registry.view<RealEngine::BlockTag, RealEngine::Position>();
    if (blockTagEntities.empty()) {
        return;
    }
    for (auto entity : blockTagEntities) {
        if (!entity) {
            continue;
        }
        auto position = _registry.get_component<RealEngine::Position>(entity);
        if (!position) {
            continue;
        }
        if (position->x > INT_MAX) {
            continue;
        }
        if (position->x < 0) {
            _registry.remove_entity(entity);
            continue;
        }
        position->x -= _ClientScrollingSpeed * fixedTimeStep;
    }
}
