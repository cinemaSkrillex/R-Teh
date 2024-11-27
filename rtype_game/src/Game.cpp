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
      _entity1(_registry.spawn_entity()), _entity2(_registry.spawn_entity()),
      _groundEntity(_registry.spawn_entity()) {
    init_registry();
    init_controls();
    init_systems();
    _idleSpaceship.setScale(3, 3);
    _upSpaceship.setScale(3, 3);
    _downSpaceship.setScale(3, 3);
    _groundSprite.setScale(3, 3);

    _spaceshipSheet.emplace("up", _upSpaceship);
    _spaceshipSheet.emplace("idle", _idleSpaceship);
    _spaceshipSheet.emplace("down", _downSpaceship);

    _registry.add_components(_entity1, Position{100.f, 100.f}, Drawable{});
    _registry.add_component(_entity1, Sprite{_idleSpaceship});

    _registry.add_component(_entity2, Position{200.f, 200.f});
    _registry.add_component(_entity2, Velocity{0.0f, 0.0f});
    _registry.add_component(_entity2, Acceleration{10.0f, 10.0f, 10.0f, true});
    _registry.add_component(_entity2, Controllable{});
    _registry.add_component(_entity2, Drawable{});
    _registry.add_component(_entity2,
                            SpriteSheet{_spaceshipSheet, "idle", 0, {32, 15}, false, false, 100});

    _registry.add_components(_groundEntity, Position{400.f, 300.f}, Drawable{});
    _registry.add_component(_groundEntity, Sprite{_groundSprite});
    // _registry.add_component(_groundEntity, Collision{{0.f, 0.f, 100.f, 100.f}, "sol", false});
}

Game::~Game() {}

void Game::init_registry() {
    _registry.register_component<Position>();
    _registry.register_component<Velocity>();
    _registry.register_component<Sprite>();
    _registry.register_component<SpriteSheet>();
    _registry.register_component<Drawable>();
    _registry.register_component<Controllable>();
    _registry.register_component<Acceleration>();
}

void Game::init_controls() {
    _controlSystem.bindKey(sf::Keyboard::Z, Action::Up);
    _controlSystem.bindKey(sf::Keyboard::S, Action::Down);
    _controlSystem.bindKey(sf::Keyboard::Q, Action::Left);
    _controlSystem.bindKey(sf::Keyboard::D, Action::Right);
    _controlSystem.bindKey(sf::Keyboard::Space, Action::Action1);
    _controlSystem.setActionHandler(
        Action::Up, std::bind(&rtype::Controls::moveUp, &_controls, std::placeholders::_1,
                              std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionHandler(Action::Down,
                                    std::bind(&rtype::Controls::moveDown, &_controls,
                                              std::placeholders::_1, std::placeholders::_2,
                                              std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionHandler(Action::Left,
                                    std::bind(&rtype::Controls::moveLeft, &_controls,
                                              std::placeholders::_1, std::placeholders::_2,
                                              std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionHandler(Action::Right,
                                    std::bind(&rtype::Controls::moveRight, &_controls,
                                              std::placeholders::_1, std::placeholders::_2,
                                              std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionHandler(Action::Action1,
                                    std::bind(&rtype::Controls::shoot, &_controls,
                                              std::placeholders::_1, std::placeholders::_2,
                                              std::placeholders::_3, std::placeholders::_4));

    _controlSystem.setActionReleaseHandler(
        Action::Up, std::bind(&rtype::Controls::decelerateUp, &_controls, std::placeholders::_1,
                              std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionReleaseHandler(Action::Down,
                                           std::bind(&rtype::Controls::decelerateDown, &_controls,
                                                     std::placeholders::_1, std::placeholders::_2,
                                                     std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionReleaseHandler(Action::Left,
                                           std::bind(&rtype::Controls::decelerateLeft, &_controls,
                                                     std::placeholders::_1, std::placeholders::_2,
                                                     std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionReleaseHandler(Action::Right,
                                           std::bind(&rtype::Controls::decelerateRight, &_controls,
                                                     std::placeholders::_1, std::placeholders::_2,
                                                     std::placeholders::_3, std::placeholders::_4));
    _controlSystem.setActionReleaseHandler(Action::Action1,
                                           std::bind(&rtype::Controls::voidAction, &_controls,
                                                     std::placeholders::_1, std::placeholders::_2,
                                                     std::placeholders::_3, std::placeholders::_4));
}

void Game::init_systems() {
    _registry.add_system<Position, Velocity>(
        [this](Registry& registry, float deltaTime, auto& positions, auto& velocities) {
            _movementSystem.update(registry, positions, velocities, deltaTime);
        });

    _registry.add_system<Position, Drawable, Sprite, SpriteSheet>(
        [this](Registry& registry, float deltaTime, auto& positions, auto& drawables, auto& sprites,
               auto& spritesheets) {
            _drawSystem.update(registry, deltaTime, positions, drawables, sprites, spritesheets);
        });

    _registry.add_system<Velocity, Controllable, Acceleration, Position>(
        [this](Registry& registry, float deltaTime, auto& velocities, auto& controllables,
               auto& accelerations, auto& positions) {
            _controlSystem.update(registry, velocities, controllables, accelerations, positions,
                                  deltaTime);
        });
    // _registry.add_system<Collision, Sprite, SpriteSheet>([this](Registry& registry, float
    // deltaTime,
    //                                                             auto& collisions, auto& sprites,
    //                                                             auto& spritesheets) {
    //     _collisionSystem.update(registry, collisions, sprites, spritesheets, deltaTime);
    // });
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