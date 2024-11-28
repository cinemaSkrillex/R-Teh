#include "Game.hpp"

namespace rtype {

Game::Game()
    : _deltaTime(0.f), _window("SKRILLEX", sf::Vector2u(800, 600)), _clock(), _controls(_registry),
      _movementSystem(), _drawSystem(_window.getRenderWindow()), _controlSystem(),
      _collisionSystem(), _view({100, 100}, {800, 600}),
      _upSpaceship("../assets/spaceship.png", sf::IntRect{0, 0, 32 * 2, 15}),
      _idleSpaceship("../assets/spaceship.png", sf::IntRect{0, 15, 32, 15}),
      _downSpaceship("../assets/spaceship.png", sf::IntRect{0, 15 * 2, 33 * 2, 15}),
      _groundSprite("../assets/r-type_front_line_base_obstacle_1.png"),
      _ennemySprite("../assets/sprites/secret_boss.png"), _entity1(_registry.spawn_entity()),
      _entity2(_registry.spawn_entity()), _groundEntity(_registry.spawn_entity()),
      _ennemyEntity(_registry.spawn_entity()) {
    init_registry();
    init_controls();
    init_systems();
    _idleSpaceship.setScale(3, 3);
    _upSpaceship.setScale(3, 3);
    _downSpaceship.setScale(3, 3);
    _groundSprite.setScale(3, 3);
    _ennemySprite.setScale(3, 3);

    _spaceshipSheet.emplace("up", _upSpaceship);
    _spaceshipSheet.emplace("idle", _idleSpaceship);
    _spaceshipSheet.emplace("down", _downSpaceship);

    _registry.add_components(_entity1, RealEngine::Position{100.f, 100.f}, RealEngine::Drawable{});
    _registry.add_component(_entity1, RealEngine::SpriteComponent{_idleSpaceship});

    _registry.add_component(_entity2, RealEngine::Position{200.f, 200.f});
    _registry.add_component(_entity2, RealEngine::Velocity{0.0f, 0.0f});
    _registry.add_component(_entity2, RealEngine::Acceleration{10.0f, 10.0f, 10.0f, true});
    _registry.add_component(_entity2, RealEngine::Controllable{});
    _registry.add_component(_entity2, RealEngine::Drawable{});
    _registry.add_component(
        _entity2, RealEngine::SpriteSheet{_spaceshipSheet, "idle", 0, {32, 15}, false, false, 100});

    _registry.add_components(_groundEntity, RealEngine::Position{400.f, 300.f},
                             RealEngine::Drawable{});
    _registry.add_component(_groundEntity, RealEngine::SpriteComponent{_groundSprite});
    // _registry.add_component(_groundEntity, Collision{{0.f, 0.f, 100.f, 100.f}, "sol", false});

    _registry.add_components(_ennemyEntity, RealEngine::Position{500.f, 300.f},
                             RealEngine::Drawable{});
    _registry.add_component(_ennemyEntity, RealEngine::SpriteComponent{_ennemySprite});
    _registry.add_component(_ennemyEntity, RealEngine::Velocity{0.0f, 0.0f});
    _registry.add_component(_ennemyEntity, RealEngine::Acceleration{10.0f, 10.0f, 10.0f, true});
    // _registry.add_component(_ennemyEntity, RealEngine::Rotation{0.0f});
}

Game::~Game() {}

void Game::init_registry() {
    _registry.register_component<RealEngine::Position>();
    _registry.register_component<RealEngine::Velocity>();
    _registry.register_component<RealEngine::SpriteComponent>();
    _registry.register_component<RealEngine::SpriteSheet>();
    _registry.register_component<RealEngine::Drawable>();
    _registry.register_component<RealEngine::Controllable>();
    _registry.register_component<RealEngine::Acceleration>();
    // _registry.register_component<RealEngine::AI>();
    // _registry.register_component<RealEngine::Rotation>();
    // _registry.register_component<RealEngine::Radius>();
}

void Game::init_controls() {
    _controlSystem.bindKey(sf::Keyboard::Z, RealEngine::Action::Up);
    _controlSystem.bindKey(sf::Keyboard::S, RealEngine::Action::Down);
    _controlSystem.bindKey(sf::Keyboard::Q, RealEngine::Action::Left);
    _controlSystem.bindKey(sf::Keyboard::D, RealEngine::Action::Right);
    _controlSystem.bindKey(sf::Keyboard::Space, RealEngine::Action::Action1);
    _controlSystem.setActionHandler(RealEngine::Action::Up,
                                    std::bind(&rtype::Controls::moveUp, &_controls,
                                              std::placeholders::_1, std::placeholders::_2,
                                              std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionHandler(RealEngine::Action::Down,
                                    std::bind(&rtype::Controls::moveDown, &_controls,
                                              std::placeholders::_1, std::placeholders::_2,
                                              std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionHandler(RealEngine::Action::Left,
                                    std::bind(&rtype::Controls::moveLeft, &_controls,
                                              std::placeholders::_1, std::placeholders::_2,
                                              std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionHandler(RealEngine::Action::Right,
                                    std::bind(&rtype::Controls::moveRight, &_controls,
                                              std::placeholders::_1, std::placeholders::_2,
                                              std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionHandler(RealEngine::Action::Action1,
                                    std::bind(&rtype::Controls::shoot, &_controls,
                                              std::placeholders::_1, std::placeholders::_2,
                                              std::placeholders::_3, std::placeholders::_4));

    _controlSystem.setActionReleaseHandler(RealEngine::Action::Up,
                                           std::bind(&rtype::Controls::decelerateUp, &_controls,
                                                     std::placeholders::_1, std::placeholders::_2,
                                                     std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionReleaseHandler(RealEngine::Action::Down,
                                           std::bind(&rtype::Controls::decelerateDown, &_controls,
                                                     std::placeholders::_1, std::placeholders::_2,
                                                     std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionReleaseHandler(RealEngine::Action::Left,
                                           std::bind(&rtype::Controls::decelerateLeft, &_controls,
                                                     std::placeholders::_1, std::placeholders::_2,
                                                     std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionReleaseHandler(RealEngine::Action::Right,
                                           std::bind(&rtype::Controls::decelerateRight, &_controls,
                                                     std::placeholders::_1, std::placeholders::_2,
                                                     std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionReleaseHandler(RealEngine::Action::Action1,
                                           std::bind(&rtype::Controls::voidAction, &_controls,
                                                     std::placeholders::_1, std::placeholders::_2,
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
    // _registry.add_system<Collision, SpriteComponent, SpriteSheet>([this](Registry& registry,
    // float deltaTime,
    //                                                             auto& collisions, auto& sprites,
    //                                                             auto& spritesheets) {
    //     _collisionSystem.update(registry, collisions, sprites, spritesheets, deltaTime);
    // });
    // _registry.add_system<RealEngine::AI>(
    //     [this](RealEngine::Registry& registry, float deltaTime, auto& ais) {
    //         _aiSystem.update(registry, deltaTime);
    //     });
    // _registry.add_system<RealEngine::Rotation>(
    //     [this](RealEngine::Registry& registry, float deltaTime, auto& rotations) {
    //         _rotationSystem.update(registry, deltaTime);
    //     });
    // _registry.add_system<RealEngine::Radius>(
    //     [this](RealEngine::Registry& registry, float deltaTime, auto& radii) {
    //         _radiusSystem.update(registry, deltaTime);
    //     });
}

void Game::run() {
    // std::unordered_map<std::string, Entity> entities = {
    // {"spaceship1", _entity1}, {"spaceship2", _entity2}, {"ground", _groundEntity}};

    while (_window.isOpen()) {
        _window.update();
        _deltaTime = _clock.restart().asSeconds();
        _window.clear();
        _registry.run_systems(_deltaTime);
        // handle_collision(_registry, entities);
        _window.display();
    }
}
} // namespace rtype