/*
** EPITECH PROJECT, 2024
** game_init
** File description:
** GameInit
*/

#include "Game.hpp"

namespace rtype {

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
        _destructibleSystem.update(registry, deltaTime);
    });
}

void Game::register_components() {
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