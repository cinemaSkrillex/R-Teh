/*
** EPITECH PROJECT, 2024
**
** File description:
** GameRun
*/

#include "Game/Game.hpp"

void rtype::Game::run() {
    while (_window.isOpen()) {
        runMenu();
        runGame();
    }
    exit(0);
}

void rtype::Game::runGame() {
    while (_window.isOpen()) {
        if (_broadcastClock.getElapsedTime().asMilliseconds() > 1000 / 10) {
            _broadcastClock.restart();
            for (int i = 0; i < 100; i++) {
                handleSignal(_clientUDP->get_last_reliable_packet_data());
                handleSignal(_clientUDP->get_last_unreliable_packet_data());
            }
        }

        if (_clock.getElapsedTime().asMilliseconds() <= 1000 / 60) continue;

        _deltaTime = _clock.restart().asSeconds();
        _window.update(_deltaTime);
        _window.clear();
        const sf::IntRect direction = getPlayerNormalizedDirection();
        _registry.run_systems(_deltaTime);
        _game_map.updateLevel(_deltaTime);
        _playerUI.update();

        _temporaryTexts.erase(
            std::remove_if(_temporaryTexts.begin(), _temporaryTexts.end(),
                           [](const std::shared_ptr<RealEngine::TemporaryText>& text) {
                               return text->isFinished();
                           }),
            _temporaryTexts.end());

        for (auto& text : _temporaryTexts) {
            text->update(_deltaTime);
            text->draw(_window.getRenderTexture());
        }

        _window.display();
        auto client_now = std::chrono::steady_clock::now();
        long client_elapsed_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(client_now - _startTime).count();
        long                                  delta_time = client_elapsed_time - _serverTime;
        RTypeProtocol::PlayerDirectionMessage playerDirectionMessage;
        playerDirectionMessage.message_type = RTypeProtocol::PLAYER_DIRECTION;
        playerDirectionMessage.uuid         = _localPlayerUUID;
        playerDirectionMessage.direction    = direction;
        playerDirectionMessage.timestamp    = delta_time;

        std::array<char, 800> serializedPlayerDirectionMessage =
            RTypeProtocol::serialize<800>(playerDirectionMessage);

        _clientUDP->send_unreliable_packet(serializedPlayerDirectionMessage);
    }
    _deltaTime = 0.0f;
}

void rtype::Game::runMenu() {
    while (_window.isOpen()) {
        _deltaTime = _clock.restart().asSeconds();
        if (!_gameMenu.run(_deltaTime)) {
            break;
        }
    }
    _deltaTime = 0.0f;
}