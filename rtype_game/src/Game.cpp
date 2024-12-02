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
      _controlSystem(),
      _collisionSystem(),
      _aiSystem(),
      _rotationSystem(),
      _radiusSystem(),
      _view({100, 100}, {800, 600}),
      _upSpaceship("../assets/spaceship.png", sf::IntRect{0, 0, 32 * 2, 15}),
      _idleSpaceship("../assets/spaceship.png", sf::IntRect{0, 15, 32, 15}),
      _downSpaceship("../assets/spaceship.png", sf::IntRect{0, 15 * 2, 33 * 2, 15}),
      _groundSprite("../assets/r-type_front_line_base_obstacle_1.png"),
      _testSprite("../assets/sprites/the_eye/boss.png", sf::IntRect{0, 0, 55, 68}),
      _entity1(_registry.spawn_entity()),
      _entity2(_registry.spawn_entity()),
      _groundEntity(_registry.spawn_entity()) {
    init_registry();
    init_controls();
    init_systems();
    _idleSpaceship.setScale(3, 3);
    _upSpaceship.setScale(3, 3);
    _downSpaceship.setScale(3, 3);
    _groundSprite.setScale(3, 3);
    _testSprite.setScale(3, 3);

    _spaceshipSheet.emplace("up", _upSpaceship);
    _spaceshipSheet.emplace("idle", _idleSpaceship);
    _spaceshipSheet.emplace("down", _downSpaceship);

    _registry.add_component(_entity2, RealEngine::Position{200.f, 200.f});
    _registry.add_component(_entity2, RealEngine::Velocity{0.0f, 0.0f, 3.0f});
    _registry.add_component(_entity2, RealEngine::Acceleration{10.0f, 10.0f, 10.0f});
    _registry.add_component(_entity2, RealEngine::Controllable{});
    _registry.add_component(_entity2, RealEngine::Drawable{});
    _registry.add_component(
        _entity2, RealEngine::SpriteSheet{_spaceshipSheet, "idle", 0, {32, 15}, false, false, 100});

    _registry.add_components(_groundEntity, RealEngine::Position{400.f, 300.f},
                             RealEngine::Drawable{});
    _registry.add_component(_groundEntity, RealEngine::SpriteComponent{_groundSprite});
    _bossEye = std::make_unique<EyeBoss>(_registry);
    _bossEye->setTarget(_entity2);
    _registry.add_component(_groundEntity,
                            RealEngine::Collision{{0.f, 0.f, 100.f, 100.f}, "sol", false});
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
    std::istringstream stream(message);
    std::string key, value;

    while (stream) {
        // Extract key
        if (!std::getline(stream, key, ':')) break;

        // Extract value (up to the next space or end of string)
        if (!std::getline(stream, value, ' ')) break;

        // Trim leading and trailing spaces from key and value
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        // Ensure both key and value are properly copied into the map
        parsedData[key] = value;
    }

    return parsedData;
}

sf::Vector2i parsePosition(const std::string& positionStr) {
    sf::Vector2i position(0, 0); // Default to (0, 0) in case of a parsing error

    // Use a regular expression to extract the values inside "(x,y)"
    std::regex positionRegex(R"(\((\d+),(\d+)\))");
    std::smatch match;

    if (std::regex_search(positionStr, match, positionRegex) && match.size() == 3) {
        // Convert the extracted strings to integers
        position.x = std::stoi(match[1].str());
        position.y = std::stoi(match[2].str());
    } else {
        std::cerr << "Failed to parse position: " << positionStr << std::endl;
    }

    return position;
}

void Game::handleSignal(std::string signal) {
    if (signal.empty())
        return;

    std::cout << "[" << signal << "]" << std::endl;
    std::unordered_map<std::string, std::string> parsedPacket = parseMessage(signal);

    if (parsedPacket.find("Event") != parsedPacket.end()) {
        const std::string event = parsedPacket.at("Event");
        if (event == "New_client") {
            const sf::Vector2i position = parsePosition(parsedPacket.at("Position"));
            spawn_player();
        } else if (event == "Synchronize") {
            const sf::Vector2i position = parsePosition(parsedPacket.at("Position"));
            spawn_player();
        }
    }
}

void Game::spawn_player() {
    RealEngine::Entity player = _registry.spawn_entity();
    _registry.add_component(player, RealEngine::Position{0.f, 0.f});
    _registry.add_component(player, RealEngine::Velocity{0.0f, 0.0f, 3.0f});
    _registry.add_component(player, RealEngine::Acceleration{10.0f, 10.0f, 10.0f});
    _registry.add_component(player, RealEngine::Controllable{});
    _registry.add_component(player, RealEngine::Drawable{});
    _registry.add_component(
        player, RealEngine::SpriteSheet{_spaceshipSheet, "idle", 0, {32, 15}, false, false, 100});
}

void Game::run() {
    std::unordered_map<std::string, RealEngine::Entity> entities = {
        {"spaceship1", _entity1}, {"spaceship2", _entity2}, {"ground", _groundEntity}};
    while (_window.isOpen()) {
        _window.update();
        _deltaTime = _clock.restart().asSeconds();
        _window.clear();
        const std::string serverEventsMessage = _clientUDP->get_last_reliable_packet();
        handleSignal(serverEventsMessage);
        _registry.run_systems(_deltaTime);
        handle_collision(_registry, entities);
        _window.display();
    }
}
}  // namespace rtype
