#include "Game.hpp"

namespace rtype {

void Game::handleSignal(std::string signal) {
    if (signal.empty()) return;

    std::cout << "[" << signal << "]" << std::endl;
    std::unordered_map<std::string, std::string> parsedPacket = parseMessage(signal);

    if (parsedPacket.find("Event") != parsedPacket.end()) {
        const std::string event = parsedPacket.at("Event");
        if (event == "New_client") {
            const sf::Vector2f position = parsePosition(parsedPacket.at("Position"));
            const long int     uuid     = std::stol(parsedPacket.at("Uuid"));
        } else if (event == "Synchronize") {
            _localPlayerUUID                 = std::stol(parsedPacket.at("Uuid"));
            const std::string players        = parsedPacket.at("Players");
            _serverTime                      = std::stol(parsedPacket.at("Clock"));
            auto client_now                  = std::chrono::steady_clock::now();
            _startTime                       = client_now - std::chrono::milliseconds(_serverTime);
            const std::string positions      = parsedPacket.at("Position");
            sf::Vector2f      localPlayerPos = parsePosition(positions);
            _registry.add_component(_entity2,
                                    RealEngine::Position{localPlayerPos.x, localPlayerPos.y});
            const std::vector<PlayerData> datas = parsePlayerList(players);
            for (PlayerData player : datas) {
                add_player(std::stol(player.uuid), player.position);
            }
        } else if (event == "Player_position") {
            const long int     uuid     = std::stol(parsedPacket.at("Uuid"));
            const sf::Vector2f position = parsePosition(parsedPacket.at("Position"));
            _registry.add_component<RealEngine::Position>(_entity2, {position.x, position.y});
        }
    }
}

RealEngine::Entity* Game::add_player(long int player_uuid, sf::Vector2f position) {
    RealEngine::Entity player = _registry.spawn_entity();
    _registry.add_component(player, RealEngine::Position{position.x, position.y});
    _registry.add_component(player, RealEngine::Drawable{});

    _registry.add_component(player, RealEngine::SpriteComponent{_otherPlayer});

    _players.emplace(player_uuid, player);
    return &_players.at(player_uuid);
}

sf::Vector2f Game::getPlayerNormalizedDirection() {
    sf::Vector2f direction(0, 0);

    if (_window.isFocused()) {
        if (sf::Keyboard::isKeyPressed(_controlSystem.getBoundKey(RealEngine::Action::Up))) {
            direction.y = -1;
        }
        if (sf::Keyboard::isKeyPressed(_controlSystem.getBoundKey(RealEngine::Action::Down))) {
            direction.y = 1;
        }
        if (sf::Keyboard::isKeyPressed(_controlSystem.getBoundKey(RealEngine::Action::Left))) {
            direction.x = -1;
        }
        if (sf::Keyboard::isKeyPressed(_controlSystem.getBoundKey(RealEngine::Action::Right))) {
            direction.x = 1;
        }
    }
    return direction;
}

void Game::run() {
    std::unordered_map<std::string, RealEngine::Entity> entities = {
        {"spaceship", _entity2}, {"ground", _groundBlocksEntities[3]}};
    while (_window.isOpen()) {
        if (_clock.getElapsedTime().asMilliseconds() <= 1000 / 60) continue;

        _deltaTime = _clock.restart().asSeconds();
        _window.update();
        _window.clear();
        // _view.move({50.0f * _deltaTime, 0});
        const std::string serverEventsMessage = _clientUDP->get_last_reliable_packet();
        handleSignal(serverEventsMessage);
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
}  // namespace rtype
