/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameInit
*/

#include "../include/Game/GameInstance.hpp"

GameInstance::GameInstance()
    : _clock(),
      _registry(),
      _drawSystem(nullptr),
      _movementSystem(),
      _collisionSystem(),
      _aiSystem(),
      _rotationSystem(),
      _radiusSystem(),
      _healthSystem(),
      _netvarSystem() {
    init_components();
    init_systems();
    init_mobs();
    init_textures();
    _upSpaceship   = RealEngine::Sprite(_textures["spaceship_up"]);
    _idleSpaceship = RealEngine::Sprite(_textures["spaceship_idle"]);
    _downSpaceship = RealEngine::Sprite(_textures["spaceship_down"]);
    _bulletSprite  = RealEngine::Sprite(_textures["bullet"]);
    _idleSpaceship.setScale(GAME_SCALE, GAME_SCALE);
    _upSpaceship.setScale(GAME_SCALE, GAME_SCALE);
    _downSpaceship.setScale(GAME_SCALE, GAME_SCALE);
    _eyeBomberSprite.setScale(GAME_SCALE, GAME_SCALE);
    _eyeBomberSprite.setOrigin(15, 10);
    _spaceshipSheet.emplace("idle", _idleSpaceship);
    _spaceshipSheet.emplace("up", _upSpaceship);
    _spaceshipSheet.emplace("down", _downSpaceship);
    std::cout << "GameInstance created" << std::endl;
}

void GameInstance::init_components() {
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
    _registry.register_component<RealEngine::AutoDestructible>();
    _registry.register_component<RealEngine::Damage>();
    _registry.register_component<RealEngine::Netvar>();
}

void GameInstance::init_systems() {
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _netvarSystem.update(registry, deltaTime);
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
        _drawSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _destructibleSystem.update(registry, deltaTime);
    });
}

void GameInstance::init_mobs() {
    // _simpleMobs.push_back(addAndGetSimpleMob({500, 100}, {-1, 0}, 50));
    // _simpleMobs.push_back(addAndGetSimpleMob({500, 200}, {-1, 0}, 35));
}

void GameInstance::init_textures() {
    _textures["spaceship_up"] = std::make_shared<sf::Texture>();
    if (!_textures["spaceship_up"]->loadFromFile("../../assets/spaceship.png",
                                                 sf::IntRect{0, 0, 32 * 2, 15})) {
        std::cerr << "Error: Could not load spaceship texture!" << std::endl;
        _textures["spaceship_up"].reset();  // Explicitly nullify the pointer
    }

    _textures["spaceship_idle"] = std::make_shared<sf::Texture>();
    if (!_textures["spaceship_idle"]->loadFromFile("../../assets/spaceship.png",
                                                   sf::IntRect{0, 15, 32, 15})) {
        std::cerr << "Error: Could not load spaceship texture!" << std::endl;
        _textures["spaceship_idle"].reset();  // Explicitly nullify the pointer
    }

    _textures["spaceship_down"] = std::make_shared<sf::Texture>();
    if (!_textures["spaceship_down"]->loadFromFile("../../assets/spaceship.png",
                                                   sf::IntRect{0, 15 * 2, 33 * 2, 15})) {
        std::cerr << "Error: Could not load spaceship texture!" << std::endl;
        _textures["spaceship_down"].reset();  // Explicitly nullify the pointer
    }

    _textures["spaceship"] = std::make_shared<sf::Texture>();
    if (!_textures["spaceship"]->loadFromFile("../../assets/spaceship.png",
                                              sf::IntRect{0, 15, 32, 15})) {
        std::cerr << "Error: Could not load spaceship texture!" << std::endl;
        _textures["spaceship"].reset();
    }

    _textures["background"] = std::make_shared<sf::Texture>();
    if (!_textures["background"]->loadFromFile(
            "../../assets/r-type_background_front_line_base_4.png")) {
        std::cerr << "Error: Could not load background texture!" << std::endl;
        _textures["background"].reset();
    }

    _textures["enemy"] = std::make_shared<sf::Texture>();
    if (!_textures["enemy"]->loadFromFile("../../assets/sprites/the_eye/bomber.png")) {
        std::cerr << "Error: Could not load enemy texture!" << std::endl;
        _textures["enemy"].reset();
    }

    _textures["bullet"] = std::make_shared<sf::Texture>();
    if (!_textures["bullet"]->loadFromFile("../../assets/spaceship_bullet.png")) {
        std::cerr << "Error: Could not load bullet texture!" << std::endl;
        _textures["bullet"].reset();
    }
}