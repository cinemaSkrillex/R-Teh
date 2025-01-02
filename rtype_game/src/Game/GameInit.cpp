/*
** EPITECH PROJECT, 2024
** game_init
** File description:
** GameInit
*/

#include "Game.hpp"

namespace rtype {

Game::Game(std::shared_ptr<UDPClient> clientUDP, unsigned short client_port)
    : _clientUDP(clientUDP),
      _deltaTime(0.f),
      _window("SKRILLEX client_port: " + std::to_string(client_port), sf::Vector2u(800, 600)),
      _clock(),
      _controls(_registry, clientUDP),
      _lagCompensationSystem(),
      _movementSystem(),
      _drawSystem(&_window.getRenderWindow()),
      _controlSystem(_window),
      _collisionSystem(),
      _aiSystem(),
      _rotationSystem(),
      _radiusSystem(),
      _healthSystem(),
      _parallaxSystem(),
      _view(_window.getRenderWindow(), {800 / 2, 600 / 2}, {800, 600}),
      _player_entity(_registry.spawn_entity()),
      _background(_registry.spawn_entity()),
      _localPlayerUUID(0),
      _startTime(std::chrono::steady_clock::now()) {
    init_all_game();
    init_player_entity();
    _registry.add_component(_background, RealEngine::Position{0.f, 0.f});
    auto backgroundSprite = RealEngine::Sprite{_textures["background"]};
    backgroundSprite.setOrigin(0, 0.5f);
    _registry.add_component(_background, RealEngine::SpriteComponent{backgroundSprite});
    _registry.add_component(_background,
                            RealEngine::Parallax{-200.f, sf::Vector2f(800.0f, 600.0f)});
    _registry.add_component(_background, RealEngine::Drawable{});

    // Example usage in game logic
    auto entity_particle = _registry.spawn_entity();
    _registry.add_component(entity_particle, RealEngine::ParticleEmitter{{},
                                                                         {100, 100},
                                                                         {10, 0},
                                                                         2.0f,
                                                                         5.0f,
                                                                         sf::Color::White,
                                                                         sf::Color::Transparent,
                                                                         2.0f,
                                                                         0.0f});
    SpacePlane mob(_registry, {1900.f, 100.f}, {1.f, 0.f}, 100.f, _mob_sprite);
    auto       mob_entity = mob.getEntity();
}

Game::~Game() {}

void Game::init_all_game() {
    init_registry();
    init_controls();
    init_systems();
    init_textures();
    init_sprites();
}

void Game::init_registry() { register_components(); }

void Game::init_controls() {
    bind_keys();
    set_action_handlers();
}

void Game::init_systems() { add_systems(); }

void Game::init_textures() {
    _textures["spaceship_up"] = std::make_shared<sf::Texture>();
    if (!_textures["spaceship_up"]->loadFromFile("../../assets/sprites/spaceship.png",
                                                 sf::IntRect{0, 0, 32 * 2, 15})) {
        std::cerr << "Error: Could not load spaceship texture!" << std::endl;
        _textures["spaceship_up"].reset();  // Explicitly nullify the pointer
    }

    _textures["spaceship_idle"] = std::make_shared<sf::Texture>();
    if (!_textures["spaceship_idle"]->loadFromFile("../../assets/sprites/spaceship.png",
                                                   sf::IntRect{0, 15, 32, 15})) {
        std::cerr << "Error: Could not load spaceship texture!" << std::endl;
        _textures["spaceship_idle"].reset();  // Explicitly nullify the pointer
    }

    _textures["spaceship_down"] = std::make_shared<sf::Texture>();
    if (!_textures["spaceship_down"]->loadFromFile("../../assets/sprites/spaceship.png",
                                                   sf::IntRect{0, 15 * 2, 33 * 2, 15})) {
        std::cerr << "Error: Could not load spaceship texture!" << std::endl;
        _textures["spaceship_down"].reset();  // Explicitly nullify the pointer
    }

    _textures["spaceship"] = std::make_shared<sf::Texture>();
    if (!_textures["spaceship"]->loadFromFile("../../assets/sprites/spaceship.png",
                                              sf::IntRect{0, 15, 32, 15})) {
        std::cerr << "Error: Could not load spaceship texture!" << std::endl;
        _textures["spaceship"].reset();
    }

    _textures["background"] = std::make_shared<sf::Texture>();
    if (!_textures["background"]->loadFromFile(
            "../../assets/sprites/backgrounds/r-type_background_front_line_base_4.png")) {
        std::cerr << "Error: Could not load background texture!" << std::endl;
        _textures["background"].reset();
    }
    _textures["background"]->setRepeated(true);

    _textures["enemy"] = std::make_shared<sf::Texture>();
    if (!_textures["enemy"]->loadFromFile("../../assets/sprites/enemies/bomber.png",
                                          {0, 0, 15, 10})) {
        std::cerr << "Error: Could not load enemy texture!" << std::endl;
        _textures["enemy"].reset();
    }

    _textures["bullet"] = std::make_shared<sf::Texture>();
    if (!_textures["bullet"]->loadFromFile("../../assets/sprites/spaceship_bullet.png")) {
        std::cerr << "Error: Could not load bullet texture!" << std::endl;
        _textures["bullet"].reset();
    }

    _textures["space_plane"] = std::make_shared<sf::Texture>();
    if (!_textures["space_plane"]->loadFromFile("../../assets/sprites/enemies/space_plane.png")) {
        std::cerr << "Error: Could not load space_plane texture!" << std::endl;
        _textures["space_plane"].reset();
    }
}

void Game::init_sprites() {
    _mob_sprite    = RealEngine::Sprite(_textures["space_plane"]);
    _upSpaceship   = RealEngine::Sprite(_textures["spaceship_up"]);
    _idleSpaceship = RealEngine::Sprite(_textures["spaceship_idle"]);
    _downSpaceship = RealEngine::Sprite(_textures["spaceship_down"]);
    _otherPlayer   = RealEngine::Sprite(_textures["spaceship"]);
    set_sprite_scales();
    set_sprite_opacity();
    populate_sprite_sheet();
}

void Game::add_systems() {
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _lagCompensationSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _controlSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _aiSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _rotationSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _collisionSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _movementSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _radiusSystem.update(registry);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _healthSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _parallaxSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _drawSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _destructibleSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _particleSystem.update(registry, deltaTime);
    });
}

void Game::register_components() {
    _registry.register_component<RealEngine::Position>();
    _registry.register_component<RealEngine::Interpolation>();
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
    _registry.register_component<RealEngine::AutoDestructible>();
    _registry.register_component<RealEngine::Damage>();
    _registry.register_component<RealEngine::Parallax>();
    _registry.register_component<RealEngine::ParticleEmitter>();
}

void Game::bind_keys() {
    _controlSystem.bindKey(sf::Keyboard::Z, RealEngine::Action::Up);
    _controlSystem.bindKey(sf::Keyboard::S, RealEngine::Action::Down);
    _controlSystem.bindKey(sf::Keyboard::Q, RealEngine::Action::Left);
    _controlSystem.bindKey(sf::Keyboard::D, RealEngine::Action::Right);
    _controlSystem.bindKey(sf::Keyboard::Up, RealEngine::Action::Up);
    _controlSystem.bindKey(sf::Keyboard::Down, RealEngine::Action::Down);
    _controlSystem.bindKey(sf::Keyboard::Left, RealEngine::Action::Left);
    _controlSystem.bindKey(sf::Keyboard::Right, RealEngine::Action::Right);
    _controlSystem.bindKey(sf::Keyboard::Space, RealEngine::Action::Action1);
}

void Game::set_action_handlers() {
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

void Game::set_sprite_scales() {
    _mob_sprite.setScale(GAME_SCALE, GAME_SCALE);
    _idleSpaceship.setScale(GAME_SCALE, GAME_SCALE);
    _upSpaceship.setScale(GAME_SCALE, GAME_SCALE);
    _downSpaceship.setScale(GAME_SCALE, GAME_SCALE);
    _otherPlayer.setScale(GAME_SCALE, GAME_SCALE);
}

void Game::set_sprite_opacity() { _otherPlayer.setOpacity(90); }

void Game::populate_sprite_sheet() {
    _spaceshipSheet.emplace("up", _upSpaceship);
    _spaceshipSheet.emplace("idle", _idleSpaceship);
    _spaceshipSheet.emplace("down", _downSpaceship);
}

void Game::init_player_entity() {
    _registry.add_component(_player_entity, RealEngine::Position{200.f, 200.f});
    _registry.add_component(_player_entity,
                            RealEngine::Velocity{0.0f, 0.0f, {300.0f, 300.0f}, 3.0f});
    _registry.add_component(_player_entity, RealEngine::Acceleration{10.0f, 10.0f, 10.0f});
    _registry.add_component(_player_entity, RealEngine::Controllable{});
    _registry.add_component(_player_entity, RealEngine::Drawable{});
    _registry.add_component(
        _player_entity,
        RealEngine::SpriteSheet{
            _spaceshipSheet, "idle", 0, {32, 15}, false, false, 100, {-1, -1}, sf::Clock()});
    _registry.add_component(
        _player_entity,
        RealEngine::Collision{
            {0.f, 0.f, 32.f * GAME_SCALE, 15.f * GAME_SCALE},
            "spaceship",
            false,
            RealEngine::CollisionType::OTHER,
            [this](RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                   RealEngine::Entity collider, RealEngine::Entity entity) {
                player_collision_handler(collisionType, registry, collider, entity);
            }});
    _registry.add_component(_player_entity, RealEngine::Health{100, 200});
}

}  // namespace rtype