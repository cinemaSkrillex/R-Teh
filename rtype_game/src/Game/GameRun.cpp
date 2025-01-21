/*
** EPITECH PROJECT, 2024
**
** File description:
** GameRun
*/

#include "Game/Game.hpp"

void rtype::Game::run() {
    while (_window.isOpen()) {
        _gameMap->updateLevel(_deltaTime);
        if (_broadcastClock.getElapsedTime().asMilliseconds() > 1000 / 10) {
            _broadcastClock.restart();
            for (int i = 0; i < 100; i++) {
                handleSignal(_clientUDP->getLastReliablePacketData());
                handleSignal(_clientUDP->getLastUnreliablePacketData());
            }
        }

        if (_clock.getElapsedTime().asMilliseconds() <= 1000 / 60) continue;

        _deltaTime = _clock.restart().asSeconds();
        _window.update(_deltaTime);
        _window.clear();
        const sf::IntRect direction = getPlayerNormalizedDirection();
        _registry.run_systems(_deltaTime);
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
        playerDirectionMessage.messageType = RTypeProtocol::PLAYER_DIRECTION;
        playerDirectionMessage.uuid        = _localPlayerUUID;
        playerDirectionMessage.direction   = direction;
        playerDirectionMessage.timestamp   = delta_time;

        // Serialize the PlayerDirectionMessage
        std::array<char, 800> serializedPlayerDirectionMessage =
            RTypeProtocol::serialize<800>(playerDirectionMessage);

        _clientUDP->sendUnreliablePacket(serializedPlayerDirectionMessage);
    }
    exit(0);
}
