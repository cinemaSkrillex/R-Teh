#include "Game.hpp"

namespace rtype {

Game::Game(std::shared_ptr<UDPClient> clientUDP, unsigned short client_port)
    : _clientUDP(clientUDP),
      _deltaTime(0.f),
      _window("SKRILLEX client_port: " + std::to_string(client_port), sf::Vector2u(800, 600)),
      _clock(),
      _controls(_registry),
      _movementSystem(),
      _drawSystem(_window.getRenderWindow()),
      _controlSystem(_window),
      _collisionSystem(),
      _aiSystem(),
      _rotationSystem(),
      _radiusSystem(),
      _view(_window.getRenderWindow(), {800 / 2, 600 / 2}, {800, 600}),
      _upSpaceship("../../assets/spaceship.png", sf::IntRect{0, 0, 32 * 2, 15}),
      _idleSpaceship("../../assets/spaceship.png", sf::IntRect{0, 15, 32, 15}),
      _downSpaceship("../../assets/spaceship.png", sf::IntRect{0, 15 * 2, 33 * 2, 15}),
      _otherPlayer("../../assets/spaceship.png", sf::IntRect{0, 15, 32, 15}),
      _groundSprite("../../assets/r-type_front_line_base_obstacle_1.png"),
      _entity2(_registry.spawn_entity()),
      _localPlayerUUID(0),
      _startTime(std::chrono::steady_clock::now()) {
    // Inside GameInit.cpp
    init_all_game();
    //

    _registry.add_component(_entity2, RealEngine::Position{200.f, 200.f});
    _registry.add_component(_entity2, RealEngine::Velocity{0.0f, 0.0f, {300.0f, 300.0f}, 3.0f});
    _registry.add_component(_entity2, RealEngine::Acceleration{10.0f, 10.0f, 10.0f});
    _registry.add_component(_entity2, RealEngine::Controllable{});
    _registry.add_component(_entity2, RealEngine::Drawable{});
    _registry.add_component(
        _entity2, RealEngine::SpriteSheet{_spaceshipSheet, "idle", 0, {32, 15}, false, false, 100});
    _registry.add_component(
        _entity2, RealEngine::Collision{
                      {0.f, 0.f, 32.f * GAME_SCALE, 15.f * GAME_SCALE}, "spaceship", false});

    for (int i = 0; i < 50; i++) {
        RealEngine::Entity groundBlock = _registry.spawn_entity();
        _registry.add_components(groundBlock,
                                 RealEngine::Position{0.f + i * (48.f * GAME_SCALE),
                                                      i % 2 ? 540.f : (460.f + 39.f * GAME_SCALE)},
                                 RealEngine::Drawable{});
        _groundBlocksEntities.push_back(groundBlock);
        _registry.add_component(groundBlock, RealEngine::SpriteComponent{_groundSprite});
        _registry.add_component(
            groundBlock, RealEngine::Collision{
                             {0.f, 0.f, 48.f * GAME_SCALE, 39.f * GAME_SCALE}, "ground", false});
    }

    _bossEye = std::make_unique<EyeBoss>(_registry);
    _bossEye->setTarget(_entity2);
}

Game::~Game() {}

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
