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
      _destructibleSystem(),
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

    auto more_background = _registry.spawn_entity();
    _registry.add_component(more_background, RealEngine::Position{0.f, 200.f});
    auto more_background_sprite = RealEngine::Sprite{_textures["spaceship_up"]};
    more_background_sprite.setScale(20, 2);
    _registry.add_component(more_background, RealEngine::SpriteComponent{more_background_sprite});
    _registry.add_component(more_background,
                            RealEngine::Collision{{0.f, 0.f, 32.f * GAME_SCALE, 15.f * GAME_SCALE},
                                                  "spaceship_up",
                                                  false,
                                                  RealEngine::CollisionType::SOLID});
    _registry.add_component(more_background, RealEngine::Drawable{});

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
    auto mob_sprite        = RealEngine::Sprite{_textures["turret_canon"]};
    auto second_mob_sprite = RealEngine::Sprite{_textures["turret_pedestal"]};
    auto third_mob_sprite  = RealEngine::Sprite{_textures["robot_boss_backward"]};
    mob_sprite.setScale(GAME_SCALE, GAME_SCALE);
    second_mob_sprite.setScale(GAME_SCALE, GAME_SCALE);
    third_mob_sprite.setScale(GAME_SCALE, GAME_SCALE);
    // DirectionalCanon mob(_registry, {1700, 300}, {0, 0}, 0, mob_sprite, true);
    WallTurret mob(_registry, {1700, 300}, mob_sprite, second_mob_sprite, false);
    mob.setTarget(_player_entity, _registry);
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
    if (!_textures["enemy"]->loadFromFile("../../assets/sprites/enemies/eye_bomber.png",
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

    _textures["space_drill"] = std::make_shared<sf::Texture>();
    if (!_textures["space_drill"]->loadFromFile("../../assets/sprites/enemies/space_drill.png")) {
        std::cerr << "Error: Could not load space_drill texture!" << std::endl;
        _textures["space_drill"].reset();
    }

    _textures["space_sphere"] = std::make_shared<sf::Texture>();
    if (!_textures["space_sphere"]->loadFromFile("../../assets/sprites/enemies/space_sphere.png")) {
        std::cerr << "Error: Could not load space_sphere texture!" << std::endl;
        _textures["space_sphere"].reset();
    }

    _textures["turret_canon"] = std::make_shared<sf::Texture>();
    if (!_textures["turret_canon"]->loadFromFile("../../assets/sprites/enemies/turret_canon.png")) {
        std::cerr << "Error: Could not load turret canon texture!" << std::endl;
        _textures["turret_canon"].reset();
    }

    _textures["turret_pedestal"] = std::make_shared<sf::Texture>();
    if (!_textures["turret_pedestal"]->loadFromFile(
            "../../assets/sprites/enemies/turret_pedestal.png")) {
        std::cerr << "Error: Could not load turret pedestal texture!" << std::endl;
        _textures["turret_pedestal"].reset();
    }

    _textures["mob_spawner_ship"] = std::make_shared<sf::Texture>();
    if (!_textures["mob_spawner_ship"]->loadFromFile(
            "../../assets/sprites/enemies/mob_spawner.png")) {
        std::cerr << "Error: Could not load mob_spawner_ship texture!" << std::endl;
        _textures["mob_spawner_ship"].reset();
    }

    _textures["mortar_rocket"] = std::make_shared<sf::Texture>();
    if (!_textures["mortar_rocket"]->loadFromFile(
            "../../assets/sprites/enemies/mortar_rocket.png")) {
        std::cerr << "Error: Could not load mortar_rocket texture!" << std::endl;
        _textures["mortar_rocket"].reset();
    }

    _textures["robot_boss_minion"] = std::make_shared<sf::Texture>();
    if (!_textures["robot_boss_minion"]->loadFromFile(
            "../../assets/sprites/enemies/boss_minion.png")) {
        std::cerr << "Error: Could not load robot boss minion texture!" << std::endl;
        _textures["robot_boss_minion"].reset();
    }

    _textures["robot_boss_shoot"] = std::make_shared<sf::Texture>();
    if (!_textures["robot_boss_shoot"]->loadFromFile("../../assets/sprites/enemies/mini_boss.png",
                                                     sf::IntRect{0, 0, 47, 43})) {
        std::cerr << "Error: Could not load robot boss texture!" << std::endl;
        _textures["robot_boss_shoot"].reset();
    }

    _textures["robot_boss_fordward"] = std::make_shared<sf::Texture>();
    if (!_textures["robot_boss_fordward"]->loadFromFile(
            "../../assets/sprites/enemies/mini_boss.png", sf::IntRect{0, 43, 56, 54})) {
        std::cerr << "Error: Could not load robot boss texture!" << std::endl;
        _textures["robot_boss_fordward"].reset();
    }

    _textures["robot_boss_backward"] = std::make_shared<sf::Texture>();
    if (!_textures["robot_boss_backward"]->loadFromFile(
            "../../assets/sprites/enemies/mini_boss.png", sf::IntRect{0, 97, 49, 50})) {
        std::cerr << "Error: Could not load robot boss texture!" << std::endl;
        _textures["robot_boss_backward"].reset();
    }

    _textures["directional_canon"] = std::make_shared<sf::Texture>();
    if (!_textures["directional_canon"]->loadFromFile(
            "../../assets/sprites/enemies/directional_canon.png")) {
        std::cerr << "Error: Could not load directional canon texture!" << std::endl;
        _textures["directional_canon"].reset();
    }

    _textures["eye_bomber"] = std::make_shared<sf::Texture>();
    if (!_textures["eye_bomber"]->loadFromFile("../../assets/sprites/enemies/eye_bomber.png")) {
        std::cerr << "Error: Could not load eye bomber texture!" << std::endl;
        _textures["eye_bomber"].reset();
    }

    _textures["eye_minion"] = std::make_shared<sf::Texture>();
    if (!_textures["eye_minion"]->loadFromFile("../../assets/sprites/enemies/eye_minion.png")) {
        std::cerr << "Error: Could not load eye minion texture!" << std::endl;
        _textures["eye_minion"].reset();
    }

    _textures["eye_bigion_normal"] = std::make_shared<sf::Texture>();
    if (!_textures["eye_bigion_normal"]->loadFromFile("../../assets/sprites/enemies/eye_bigion.png",
                                                      sf::IntRect{0, 0, 23 * 2, 16})) {
        std::cerr << "Error: Could not load eye bigion texture!" << std::endl;
        _textures["eye_bigion_normal"].reset();
    }

    _textures["eye_bigion_angry"] = std::make_shared<sf::Texture>();
    if (!_textures["eye_bigion_angry"]->loadFromFile("../../assets/sprites/enemies/eye_bigion.png",
                                                     sf::IntRect{0, 16, 21 * 2, 16})) {
        std::cerr << "Error: Could not load eye bigion texture!" << std::endl;
        _textures["eye_bigion_angry"].reset();
    }
}

void Game::init_sprites() {
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