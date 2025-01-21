/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongGame.cpp
*/

#include "../include/Game/PongGame.hpp"

namespace pong {
Game::Game(std::shared_ptr<UDPClient> clientUDP, unsigned short clientPort)
    : _clientUDP(clientUDP),
      _deltaTime(0.f),
      _view(sf::Vector2f(VIEW_WIDTH / 2, VIEW_HEIGHT / 2), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT)),
      _window("SKRILLEX clientPort: " + std::to_string(clientPort),
              sf::Vector2u(VIEW_WIDTH, VIEW_HEIGHT), _view),
      _clock(),
      _controls(_registry, clientUDP),
      _lagCompensationSystem(),
      _movementSystem(),
      _drawSystem(&_window.getRenderTexture()),
      _controlSystem(_window),
      _playerEntity(nullptr),
      _particleSystem(),
      _netvarSystem(),
      _startTime(std::chrono::steady_clock::now()),
      _lastTimestamp(std::chrono::steady_clock::now()) {
    registerComponents();
    bindKeys();
    setActionHandlers();
    initSystems();
    initTextures();
    initMusics();
    initSounds();
    initFonts();
    initScreenLimits();
}

Game::~Game() {}

void Game::registerComponents() {
    _registry.registerComponent<RealEngine::Position>();
    _registry.registerComponent<RealEngine::Interpolation>();
    _registry.registerComponent<RealEngine::Velocity>();
    _registry.registerComponent<RealEngine::SpriteComponent>();
    _registry.registerComponent<RealEngine::SpriteSheet>();
    _registry.registerComponent<RealEngine::Drawable>();
    _registry.registerComponent<RealEngine::Collision>();
    _registry.registerComponent<RealEngine::Controllable>();
    _registry.registerComponent<RealEngine::Acceleration>();
    _registry.registerComponent<RealEngine::Parallax>();
    _registry.registerComponent<RealEngine::ParticleEmitter>();
    _registry.registerComponent<RealEngine::Particle>();
    _registry.registerComponent<RealEngine::Netvar>();
    _registry.registerComponent<RealEngine::NetvarContainer>();
    _registry.registerComponent<RealEngine::Score>();
}

void Game::bindKeys() {
    _controlSystem.bindKey(sf::Keyboard::Z, RealEngine::Action::Up);
    _controlSystem.bindKey(sf::Keyboard::S, RealEngine::Action::Down);
    _controlSystem.bindKey(sf::Keyboard::Up, RealEngine::Action::Up);
    _controlSystem.bindKey(sf::Keyboard::Down, RealEngine::Action::Down);
    _controlSystem.bindKey(sf::Keyboard::Space, RealEngine::Action::Action1);
}

void Game::setActionHandlers() {
    _controlSystem.setActionHandler(RealEngine::Action::Up,
                                    std::bind(&pong::Controls::moveUp, &_controls,
                                              std::placeholders::_1, std::placeholders::_2));
    _controlSystem.setActionHandler(RealEngine::Action::Down,
                                    std::bind(&pong::Controls::moveDown, &_controls,
                                              std::placeholders::_1, std::placeholders::_2));
    _controlSystem.setActionHandler(RealEngine::Action::Action1,
                                    std::bind(&pong::Controls::sendReady, &_controls,
                                              std::placeholders::_1, std::placeholders::_2));
}

void Game::initSystems() {
    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _lagCompensationSystem.update(registry, deltaTime);
    });
    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _controlSystem.update(registry, deltaTime);
    });
    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _movementSystem.update(registry, deltaTime);
    });
    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _drawSystem.update(registry, deltaTime);
    });
    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _particleSystem.update(registry, deltaTime);
    });
    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _netvarSystem.update(registry, deltaTime);
    });
}

void Game::initTextures() {
    std::string path = "../../assets/sprites/pong/";

    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();

    AssetManagerInstance.loadSpriteTextureAndScale("background", path + "background.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("ball", path + "ball.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("player_bar", path + "bar_idle.png",
                                                   {GAME_SCALE, GAME_SCALE});
}

void Game::initMusics() {
    // no music in pong for now
}

void Game::initSounds() {
    // no sound in pong for now
}

void Game::initFonts() {
    std::string path = "../../assets/fonts/";
    if (assetLauncher == true) {
        path = "assets/fonts/";
    }
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadFont("pixel", path + "Early_GameBoy.ttf");
}

void Game::initScreenLimits() {
    std::shared_ptr<RealEngine::Entity> topWall = _registry.spawnEntity();
    _registry.addComponent(
        topWall, RealEngine::Collision{
                     {0, -20, 800, 20}, "wall", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> bottomWall = _registry.spawnEntity();
    _registry.addComponent(
        bottomWall, RealEngine::Collision{
                        {0, 600, 800, 20}, "wall", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> leftWall = _registry.spawnEntity();
    _registry.addComponent(
        leftWall, RealEngine::Collision{
                      {-20, 0, 20, 600}, "wall", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> rightWall = _registry.spawnEntity();
    _registry.addComponent(
        rightWall, RealEngine::Collision{
                       {800, 0, 20, 600}, "wall", false, RealEngine::CollisionType::BLOCKING});
}

sf::IntRect Game::getPlayerNormalizedDirection() {
    sf::IntRect direction(0, 0, 0, 0);
    auto*       velocity = _registry.getComponent<RealEngine::Velocity>(_playerEntity);

    if (_window.isFocused() && velocity) {
        if (_controlSystem.isActionPressed(RealEngine::Action::Up)) {
            direction.width = 1;
        }
        if (_controlSystem.isActionPressed(RealEngine::Action::Down)) {
            direction.height = 1;
        }

        if (direction.width == 1 && direction.height == 1) {
            velocity->vy = 0;
        }
    }
    return direction;
}

void Game::run() {
    while (_window.isOpen()) {
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
        _window.display();

        if (_playerEntity == nullptr) continue;

        long int uuidSearch = 0;
        for (const auto& entity : _entities) {
            if (*entity.second == *_playerEntity) {
                uuidSearch = entity.first;
                break;
            }
        }
        if (uuidSearch == 0) continue;

        PongProtocol::PlayerDirectionMessage playerDirectionMessage;
        playerDirectionMessage.messageType = PongProtocol::PLAYER_DIRECTION;
        playerDirectionMessage.uuid        = uuidSearch;
        playerDirectionMessage.direction   = direction;
        playerDirectionMessage.timestamp =
            std::chrono::system_clock::now().time_since_epoch().count();

        // Serialize the PlayerDirectionMessage
        std::array<char, 800> serializedPlayerDirectionMessage =
            PongProtocol::serialize<800>(playerDirectionMessage);

        _clientUDP->sendUnreliablePacket(serializedPlayerDirectionMessage);
    }
    exit(0);
}

}  // namespace pong