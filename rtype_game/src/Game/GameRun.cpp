/*
** EPITECH PROJECT, 2024
** \
** File description:
** GameRun
*/

#include "Game.hpp"

void rtype::Game::run() {
    std::unordered_map<std::string, RealEngine::Entity> entities = {
        {"spaceship", _entity2}, {"ground", _groundBlocksEntities[3]}};
    while (_window.isOpen()) {
        if (_clock.getElapsedTime().asMilliseconds() <= 1000 / 60) continue;

        _deltaTime = _clock.restart().asSeconds();
        _window.update();
        _window.clear();
        handleSignal(_clientUDP->get_last_reliable_packet());
        handleSignal(_clientUDP->get_last_unreliable_packet());
        _registry.run_systems(_deltaTime);
        handle_collision(_registry, entities);
        const sf::Vector2f direction = getPlayerNormalizedDirection();
        _window.display();
        auto client_now = std::chrono::steady_clock::now();
        long client_elapsed_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(client_now - _startTime).count();
        long              delta_time = client_elapsed_time - _serverTime;
        const std::string message    = "Uuid:" + std::to_string(_localPlayerUUID) +
                                    " Timestamp:" + std::to_string(delta_time) + " Direction:(" +
                                    std::to_string(direction.x) + "," +
                                    std::to_string(direction.y) + ")";
        _clientUDP->send_unreliable_packet(message);
    }
    exit(0);
}