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
      _reappearingSystem(),
      _view(_window.getRenderWindow(), {800 / 2, 600 / 2}, {800, 600}),
      _upSpaceship("../../assets/spaceship.png", sf::IntRect{0, 0, 32 * 2, 15}),
      _idleSpaceship("../../assets/spaceship.png", sf::IntRect{0, 15, 32, 15}),
      _downSpaceship("../../assets/spaceship.png", sf::IntRect{0, 15 * 2, 33 * 2, 15}),
      _otherPlayer("../../assets/spaceship.png", sf::IntRect{0, 15, 32, 15}),
      _groundSprite("../../assets/r-type_front_line_base_obstacle_1.png"),
      _backgroundSprite("../../assets/r-type_background_front_line_base_4.png"),
      _eyeBomberSprite("../../assets/sprites/the_eye/bomber.png"),
      _eyeMinionSprite("../../assets/sprites/the_eye/minion.png"),
      _entity2(_registry.spawn_entity()),
      _localPlayerUUID(0),
      _startTime(std::chrono::steady_clock::now()) {
    init_all_game();

    _backgroundSprite.setScale(5.f, 5.f);
    float spriteWidth = 192.f * 5.f;

    std::shared_ptr<RealEngine::Entity> backgroundBlock1 = _registry.spawn_entity();
    _registry.add_components(backgroundBlock1, RealEngine::Position{0.f, 0.f},
                             RealEngine::Velocity{-100.f, 0.f, {100.f, 0.f}},
                             RealEngine::Reappearing{100.f, 900.f, spriteWidth},
                             RealEngine::Drawable{});
    _registry.add_component(backgroundBlock1, RealEngine::SpriteComponent{_backgroundSprite});
    _backgroundEntities.push_back(backgroundBlock1);

    std::shared_ptr<RealEngine::Entity> backgroundBlock2 = _registry.spawn_entity();
    _registry.add_components(backgroundBlock2, RealEngine::Position{spriteWidth, 0.f},
                             RealEngine::Velocity{-100.f, 0.f, {100.f, 0.f}},
                             RealEngine::Reappearing{100.f, 900.f, spriteWidth},
                             RealEngine::Drawable{});
    _registry.add_component(backgroundBlock2, RealEngine::SpriteComponent{_backgroundSprite});
    _backgroundEntities.push_back(backgroundBlock2);

    // _backgroundSprite.setScale(3.f, 3.f);

    // float spriteWidth2 = 192.f * 3.f;

    // std::shared_ptr<RealEngine::Entity> backgroundBlock3 = _registry.spawn_entity();
    // _registry.add_components(
    //     backgroundBlock3,
    //     RealEngine::Position{0.f, 0.f},
    //     RealEngine::Velocity{-50.f, 0.f, {100.f, 0.f}},
    //     RealEngine::Reappearing{25.f, 900.f, spriteWidth2},
    //     RealEngine::Drawable{}
    // );
    // _registry.add_component(backgroundBlock3, RealEngine::SpriteComponent{_backgroundSprite});
    // _backgroundEntities.push_back(backgroundBlock3);

    // std::shared_ptr<RealEngine::Entity> backgroundBlock4 = _registry.spawn_entity();
    // _registry.add_components(
    //     backgroundBlock4,
    //     RealEngine::Position{spriteWidth2, 0.f},
    //     RealEngine::Velocity{-50.f, 0.f, {100.f, 0.f}},
    //     RealEngine::Reappearing{25.f, 900.f , spriteWidth2},
    //     RealEngine::Drawable{}
    // );
    // _registry.add_component(backgroundBlock4, RealEngine::SpriteComponent{_backgroundSprite});
    // _backgroundEntities.push_back(backgroundBlock4);

    _registry.add_component(_entity2, RealEngine::Position{200.f, 200.f});
    _registry.add_component(_entity2, RealEngine::Velocity{0.0f, 0.0f, {300.0f, 300.0f}, 3.0f});
    _registry.add_component(_entity2, RealEngine::Acceleration{10.0f, 10.0f, 10.0f});
    _registry.add_component(_entity2, RealEngine::Controllable{});
    _registry.add_component(_entity2, RealEngine::Drawable{});
    _registry.add_component(
        _entity2, RealEngine::SpriteSheet{_spaceshipSheet, "idle", 0, {32, 15}, false, false, 100});
    _registry.add_component(
        _entity2,
        RealEngine::Collision{{0.f, 0.f, 32.f * GAME_SCALE, 15.f * GAME_SCALE},
                              "spaceship",
                              false,
                              RealEngine::CollisionType::OTHER,
                              [this](RealEngine::CollisionType collisionType,
                                     RealEngine::Registry& registry, RealEngine::Entity collider) {
                                  player_collision_handler(collisionType, registry, collider);
                              }});
    _registry.add_component(_entity2, RealEngine::Health{100, 200});

    for (int i = 0; i < 50; i++) {
        std::shared_ptr<RealEngine::Entity> groundBlock = _registry.spawn_entity();
        _registry.add_components(groundBlock,
                                 RealEngine::Position{0.f + i * (48.f * GAME_SCALE),
                                                      i % 2 ? 540.f : (460.f + 39.f * GAME_SCALE)},
                                 RealEngine::Drawable{});
        _groundBlocksEntities.push_back(groundBlock);
        _registry.add_component(groundBlock, RealEngine::SpriteComponent{_groundSprite});
        _registry.add_component(
            groundBlock, RealEngine::Collision{{0.f, 0.f, 48.f * GAME_SCALE, 39.f * GAME_SCALE},
                                               "ground",
                                               false,
                                               RealEngine::CollisionType::SOLID});
    }

    // _eyeMinion = std::make_unique<EyeMinion>(_registry, sf::Vector2f({200.f, 200.f}));
    auto eyeBomber =
        std::make_unique<EyeBomber>(_registry, sf::Vector2f({500.f, 200.f}), _eyeBomberSprite);
    // eyeBomber->setTarget(_entity2, _registry);
    _eyeMinions.push_back(std::move(eyeBomber));
    _bossEye = std::make_unique<EyeBoss>(_registry);
    _bossEye->setTarget(_entity2);
}

Game::~Game() {}

void Game::init_all_game() {
    init_registry();
    init_controls();
    init_systems();
    init_sprites();
}

void Game::init_registry() { register_components(); }

void Game::init_controls() {
    bind_keys();
    set_action_handlers();
}

void Game::init_systems() { add_systems(); }

void Game::init_sprites() {
    set_sprite_scales();
    set_sprite_opacity();
    populate_sprite_sheet();
}

void Game::add_systems() {
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _lagCompensationSystem.update(registry, deltaTime);
    });

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
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _healthSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _destructibleSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _reappearingSystem.update(registry, deltaTime);
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
    _registry.register_component<RealEngine::Reappearing>();
}

void Game::bind_keys() {
    _controlSystem.bindKey(sf::Keyboard::Z, RealEngine::Action::Up);
    _controlSystem.bindKey(sf::Keyboard::S, RealEngine::Action::Down);
    _controlSystem.bindKey(sf::Keyboard::Q, RealEngine::Action::Left);
    _controlSystem.bindKey(sf::Keyboard::D, RealEngine::Action::Right);
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
    _groundSprite.setScale(GAME_SCALE, GAME_SCALE);
    _otherPlayer.setScale(GAME_SCALE, GAME_SCALE);
}

void Game::set_sprite_opacity() { _otherPlayer.setOpacity(90); }

void Game::populate_sprite_sheet() {
    _spaceshipSheet.emplace("up", _upSpaceship);
    _spaceshipSheet.emplace("idle", _idleSpaceship);
    _spaceshipSheet.emplace("down", _downSpaceship);
}

}  // namespace rtype