#include "Game.hpp"

namespace rtype {

Game::Game(std::shared_ptr<UDPClient> clientUDP)
    : _clientUDP(clientUDP),
      _deltaTime(0.f),
      _window("SKRILLEX", sf::Vector2u(800, 600)),
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
    init_registry();
    init_controls();
    init_systems();
    _idleSpaceship.setScale(GAME_SCALE, GAME_SCALE);
    _upSpaceship.setScale(GAME_SCALE, GAME_SCALE);
    _downSpaceship.setScale(GAME_SCALE, GAME_SCALE);
    _groundSprite.setScale(GAME_SCALE, GAME_SCALE);
    _otherPlayer.setScale(GAME_SCALE, GAME_SCALE);
    _otherPlayer.setOpacity(90);

    _spaceshipSheet.emplace("up", _upSpaceship);
    _spaceshipSheet.emplace("idle", _idleSpaceship);
    _spaceshipSheet.emplace("down", _downSpaceship);

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

void Game::init_registry() {
    _registry.register_component<RealEngine::Position>();
    _registry.register_component<RealEngine::Velocity>();
    _registry.register_component<RealEngine::Health>();
    _registry.register_component<RealEngine::SpriteComponent>();
    _registry.register_component<RealEngine::SpriteSheet>();
    _registry.register_component<RealEngine::Drawable>();
    _registry.register_component<RealEngine::Collision>();
    _registry.register_component<RealEngine::Controllable>();
    _registry.register_component<RealEngine::Acceleration>();
    _registry.register_component<RealEngine::AI>();
    _registry.register_component<RealEngine::Rotation>();
    _registry.register_component<RealEngine::Radius>();
    _registry.register_component<RealEngine::Target>();
}

void Game::init_controls() {
    _controlSystem.bindKey(sf::Keyboard::Z, RealEngine::Action::Up);
    _controlSystem.bindKey(sf::Keyboard::S, RealEngine::Action::Down);
    _controlSystem.bindKey(sf::Keyboard::Q, RealEngine::Action::Left);
    _controlSystem.bindKey(sf::Keyboard::D, RealEngine::Action::Right);
    _controlSystem.bindKey(sf::Keyboard::Space, RealEngine::Action::Action1);
    _controlSystem.setActionHandler(
        RealEngine::Action::Up,
        std::bind(&rtype::Controls::moveUp, &_controls, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionHandler(
        RealEngine::Action::Down,
        std::bind(&rtype::Controls::moveDown, &_controls, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionHandler(
        RealEngine::Action::Left,
        std::bind(&rtype::Controls::moveLeft, &_controls, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionHandler(
        RealEngine::Action::Right,
        std::bind(&rtype::Controls::moveRight, &_controls, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionHandler(
        RealEngine::Action::Action1,
        std::bind(&rtype::Controls::shoot, &_controls, std::placeholders::_1, std::placeholders::_2,
                  std::placeholders::_3, std::placeholders::_4));
}

void Game::init_systems() {
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _movementSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _drawSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _controlSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _collisionSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _aiSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _rotationSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _radiusSystem.update(registry);
    });
}

std::unordered_map<std::string, std::string> parseMessage(const std::string& message) {
    std::unordered_map<std::string, std::string> parsedData;
    std::istringstream                           stream(message);
    std::string                                  key, value;

    while (stream) {
        if (!std::getline(stream, key, ':')) break;
        if (!std::getline(stream, value, ' ')) break;
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        // Ensure both key and value are properly copied into the map
        parsedData[key] = value;
    }

    return parsedData;
}

const sf::Vector2f parsePosition(const std::string& positionStr) {
    sf::Vector2f position(0, 0);  // Default to (0, 0) in case of a parsing error

    // Updated regex to handle floating-point numbers
    std::regex  positionRegex(R"(\(([-+]?\d*\.?\d+),([-+]?\d*\.?\d+)\))");
    std::smatch match;

    if (std::regex_search(positionStr, match, positionRegex)) {
        // Convert the extracted strings to floats
        position.x = std::stof(match[1].str());
        position.y = std::stof(match[2].str());
    } else {
        std::cerr << "Failed to parse position: " << positionStr << std::endl;
    }

    return position;
}

struct PlayerData {
    std::string  uuid;
    sf::Vector2f position;
};

// Function to parse the player list string (without the surrounding brackets)
std::vector<PlayerData> parsePlayerList(const std::string& playerList) {
    std::vector<PlayerData> players;

    // Remove the surrounding brackets
    if (playerList.size() >= 2 && playerList.front() == '[' && playerList.back() == ']') {
        std::string cleanedList =
            playerList.substr(1, playerList.length() - 2);  // Removes "[" and "]"

        // Regex to match player data
        std::regex  playerRegex(R"(\|?([^,]+),\(([-+]?\d*\.?\d+),([-+]?\d*\.?\d+)\))");
        std::smatch match;

        std::string::const_iterator searchStart(cleanedList.cbegin());
        while (std::regex_search(searchStart, cleanedList.cend(), match, playerRegex)) {
            if (match.size() == 4) {
                PlayerData player;
                player.uuid = match[1].str();
                player.position =
                    sf::Vector2f(std::stof(match[2].str()), std::stof(match[3].str()));

                // Sanitize UUID
                player.uuid.erase(std::remove(player.uuid.begin(), player.uuid.end(), '|'),
                                  player.uuid.end());

                players.push_back(player);

                std::cout << "Parsed player - UUID: " << player.uuid << ", Position: ("
                          << player.position.x << ", " << player.position.y << ")" << std::endl;
            } else {
                std::cerr << "Malformed player data: " << match.str() << std::endl;
            }

            // Move the search start to the end of the current match
            searchStart = match.suffix().first;
        }
    } else {
        std::cerr << "Malformed player list: " << playerList << std::endl;
    }

    return players;
}

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
            _localPlayerUUID                    = std::stol(parsedPacket.at("Uuid"));
            const std::string players           = parsedPacket.at("Players");
            _serverTime                         = std::stol(parsedPacket.at("Clock"));
            const std::string positions        = parsedPacket.at("Position");
            sf::Vector2f      localPlayerPos    = parsePosition(positions);
            _registry.add_component(_entity2, RealEngine::Position{localPlayerPos.x, localPlayerPos.y});
            const std::vector<PlayerData> datas = parsePlayerList(players);
            for (PlayerData player : datas) {
                add_player(std::stol(player.uuid), player.position);
            }
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
        _window.update();
        _deltaTime = _clock.restart().asSeconds();
        _window.clear();
        // _view.move({50.0f * _deltaTime, 0});
        const std::string serverEventsMessage = _clientUDP->get_last_reliable_packet();
        handleSignal(serverEventsMessage);
        _registry.run_systems(_deltaTime);
        handle_collision(_registry, entities);
        const sf::Vector2f direction  = getPlayerNormalizedDirection();
        auto               client_now = std::chrono::steady_clock::now();
        long               client_elapsed_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(client_now - _startTime).count();
        long              delta_time = client_elapsed_time - _serverTime;
        const std::string message    = "Uuid:" + std::to_string(_localPlayerUUID) +
                                    " Timestamp:" + std::to_string(delta_time) +" Direction:(" + std::to_string(direction.x) +
                                    "," + std::to_string(direction.y) + ")";
        auto* position = _registry.get_component<RealEngine::Position>(_entity2);
        std::cout << "Player" << "Position: (" << position->x << ", " << position->y << ")"
                  << std::endl;
        _clientUDP->send_unreliable_packet(message);
        _window.display();
    }
    exit(0);
}
}  // namespace rtype
