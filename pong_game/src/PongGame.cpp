/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongGame.cpp
*/

#include "../include/Game/PongGame.hpp"

namespace pong {
Game::Game(std::shared_ptr<UDPClient> clientUDP, unsigned short client_port)
    : _clientUDP(clientUDP),
      _deltaTime(0.f),
      _view(sf::Vector2f(VIEW_WIDTH / 2, VIEW_HEIGHT / 2),
            sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT + 100)),
      _window("SKRILLEX client_port: " + std::to_string(client_port),
              sf::Vector2u(VIEW_WIDTH, VIEW_HEIGHT + 100), _view),
      _clock(),
      _controls(_registry, clientUDP),
      _lagCompensationSystem(),
      _movementSystem(),
      _drawSystem(&_window.getRenderTexture()),
      _controlSystem(_window),
      _player_entity(nullptr),
      _particleSystem(),
      _netvarSystem(),
      _startTime(std::chrono::steady_clock::now()) {
    register_components();
    bind_keys();
    set_action_handlers();
    init_systems();
    init_textures();
    init_musics();
    init_sounds();
    init_fonts();
    init_screen_limits();
}

Game::~Game() {}

void Game::register_components() {
    _registry.register_component<RealEngine::Position>();
    _registry.register_component<RealEngine::Interpolation>();
    _registry.register_component<RealEngine::Velocity>();
    _registry.register_component<RealEngine::SpriteComponent>();
    _registry.register_component<RealEngine::Drawable>();
    _registry.register_component<RealEngine::Collision>();
    _registry.register_component<RealEngine::Controllable>();
    _registry.register_component<RealEngine::Acceleration>();
    _registry.register_component<RealEngine::Parallax>();
    _registry.register_component<RealEngine::ParticleEmitter>();
    _registry.register_component<RealEngine::Particle>();
    _registry.register_component<RealEngine::Netvar>();
    _registry.register_component<RealEngine::NetvarContainer>();
    _registry.register_component<RealEngine::Score>();
}

void Game::bind_keys() {
    _controlSystem.bindKey(sf::Keyboard::Z, RealEngine::Action::Up);
    _controlSystem.bindKey(sf::Keyboard::S, RealEngine::Action::Down);
    _controlSystem.bindKey(sf::Keyboard::Up, RealEngine::Action::Up);
    _controlSystem.bindKey(sf::Keyboard::Down, RealEngine::Action::Down);
    _controlSystem.bindKey(sf::Keyboard::Space, RealEngine::Action::Action1);
}

void Game::set_action_handlers() {
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

void Game::init_systems() {
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _lagCompensationSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _controlSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _movementSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _drawSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _particleSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _netvarSystem.update(registry, deltaTime);
    });
}

void Game::init_textures() {
    std::string path = "../../assets/sprites/pong/";

    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();

    AssetManagerInstance.loadSpriteTextureAndScale("background", path + "background.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("ball", path + "ball.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("player_bar", path + "bar_idle.png",
                                                   {GAME_SCALE, GAME_SCALE});
}

void Game::init_musics() {
    // no music in pong for now
}

void Game::init_sounds() {
    // no sound in pong for now
}

void Game::init_fonts() {
    std::string path = "../../assets/fonts/";
    if (assetLauncher == true) {
        path = "assets/fonts/";
    }
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadFont("pixel", path + "Early_GameBoy.ttf");
}

void Game::init_screen_limits() {
    std::shared_ptr<RealEngine::Entity> topWall = _registry.spawn_entity();
    _registry.add_component(
        topWall, RealEngine::Collision{
                     {0, -20, 800, 20}, "wall", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> bottomWall = _registry.spawn_entity();
    _registry.add_component(
        bottomWall, RealEngine::Collision{
                        {0, 600, 800, 20}, "wall", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> leftWall = _registry.spawn_entity();
    _registry.add_component(
        leftWall, RealEngine::Collision{
                      {-20, 0, 20, 600}, "wall", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> rightWall = _registry.spawn_entity();
    _registry.add_component(
        rightWall, RealEngine::Collision{
                       {800, 0, 20, 600}, "wall", false, RealEngine::CollisionType::BLOCKING});
}

void Game::run() {
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
        // const sf::IntRect direction = getPlayerNormalizedDirection();
        _registry.run_systems(_deltaTime);
        _window.display();
        // auto client_now = std::chrono::steady_clock::now();
        // long client_elapsed_time =
        //     std::chrono::duration_cast<std::chrono::milliseconds>(client_now - _startTime).count();
        // long                                  delta_time = client_elapsed_time - _serverTime;
        // PongProtocol::PlayerDirectionMessage playerDirectionMessage;
        // playerDirectionMessage.message_type = PongProtocol::PLAYER_DIRECTION;
        // playerDirectionMessage.uuid         = _localPlayerUUID;
        // playerDirectionMessage.direction    = direction;
        // playerDirectionMessage.timestamp    = delta_time;

        // // Serialize the PlayerDirectionMessage
        // std::array<char, 800> serializedPlayerDirectionMessage =
        //     PongProtocol::serialize<800>(playerDirectionMessage);

        // _clientUDP->send_unreliable_packet(serializedPlayerDirectionMessage);
    }
    exit(0);
}

}  // namespace pong