/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongGameInstance.cpp
*/

#include "GameInstance/PongGameInstance.hpp"

PongGameInstance::PongGameInstance(bool serverVision)
    : _serverVision(serverVision),
      _clock(),
      _window(_serverVision ? std::make_unique<RealEngine::Window>("I CAN SEE EVERYTHING",
                                                                   sf::Vector2u(800, 600))
                            : nullptr),
      _registry(),
      _drawSystem(_serverVision ? &_window->getRenderTexture() : nullptr),
      _movementSystem(),
      _collisionSystem(),
      _netvarSystem(),
      _particleSystem() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    init_components();
    init_systems();
    init_textures();
    init_sprite_sheets();
    init_screen_limits();
}

std::shared_ptr<RealEngine::Entity> PongGameInstance::addAndGetPlayer(sf::Vector2f position,
                                                                      size_t       id) {
    pong::PongPlayer player(_registry, position, id);
    _entities.push_back(player.getEntity());
    return _entities.back();
}

std::shared_ptr<RealEngine::Entity> PongGameInstance::addAndGetEntity(
    std::shared_ptr<RealEngine::Entity> entity) {
    _entities.push_back(entity);
    return _entities.back();
}

std::shared_ptr<RealEngine::Entity> PongGameInstance::addAndGetBall() {
    pong::PongBall ball(_registry, sf::Vector2f(400, 300));
    _entities.push_back(ball.getEntity());
    return ball.getEntity();
}

std::shared_ptr<RealEngine::Entity> PongGameInstance::addAndGetBackground() {
    auto background = _registry.spawn_entity();
    _registry.add_component(background, RealEngine::Position{400, 300});
    auto sprite = RealEngine::AssetManager::getInstance().getSprite("background");
    _registry.add_component(background, RealEngine::SpriteComponent{*sprite});
    _registry.add_component(background, RealEngine::Drawable{});
    _registry.add_component(
        background,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("background"), nullptr}}}});
    _entities.push_back(background);
    return _entities.back();
}

void PongGameInstance::movePlayer(sf::IntRect direction, float deltaTime, size_t player) {
    auto it = std::find_if(
        _entities.begin(), _entities.end(),
        [player](const std::shared_ptr<RealEngine::Entity>& entity) { return *entity == player; });

    if (it == _entities.end()) {
        return;
    }

    auto  entity       = *it;
    auto* acceleration = _registry.get_component<RealEngine::Acceleration>(entity);
    auto* velocity     = _registry.get_component<RealEngine::Velocity>(entity);

    if (!acceleration || !velocity) return;

    // if (direction.left > 0 && velocity->vx > 50) velocity->vx = 50;
    // if (direction.top > 0 && velocity->vx < -50) velocity->vx = -50;
    // if (direction.width > 0 && velocity->vy > 50) velocity->vy = 50;
    // if (direction.height > 0 && velocity->vy < -50) velocity->vy = -50;
    // if (direction.top > 0) velocity->vx += (acceleration->ax * 3 * deltaTime);
    // if (direction.left > 0) velocity->vx -= (acceleration->ax * 3 * deltaTime);
    // if (direction.width > 0) velocity->vy -= (acceleration->ay * 3 * deltaTime);
    // if (direction.height > 0) velocity->vy += (acceleration->ay * 3 * deltaTime);
    if (direction.width > 0) velocity->vy -= (acceleration->ay * deltaTime);
    if (direction.height > 0) velocity->vy += (acceleration->ay * deltaTime);
    if (direction.width == direction.height) {
        velocity->vy = 0;
    }
}

std::vector<RealEngine::Entity> PongGameInstance::run(float deltaTime) {
    if (_serverVision) {
        _window->clear();
        _window->update(deltaTime);
        _drawSystem.update(_registry, deltaTime);
        _window->display();
    } else {
        _drawSystem.updateWithoutDisplay(_registry, deltaTime);
    }
    _collisionSystem.update(_registry, deltaTime);
    _netvarSystem.update(_registry, deltaTime);
    _particleSystem.update(_registry, deltaTime);
    _movementSystem.update(_registry, deltaTime);

    return {};
}
