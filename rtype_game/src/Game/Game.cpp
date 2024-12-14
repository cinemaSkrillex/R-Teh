/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Game
*/

#include "Game.hpp"

namespace rtype {

std::shared_ptr<RealEngine::Entity> Game::add_player(long int player_uuid, sf::Vector2f position) {
    std::shared_ptr<RealEngine::Entity> player = _registry.spawn_entity();
    _registry.add_component(player, RealEngine::Position{position.x, position.y});
    _registry.add_component(player, RealEngine::Drawable{});

    _registry.add_component(player, RealEngine::Interpolation{{0, 0}, {0, 0}, 0.f, 1.f, false});

    _registry.add_component(player, RealEngine::SpriteComponent{_otherPlayer});

    _players.emplace(player_uuid, player);
    return _players.at(player_uuid);
}

std::shared_ptr<RealEngine::Entity> Game::add_mob(long int enemy_uuid, sf::Vector2f position) {
    auto enemy       = EyeBomber(_registry, position, _eyeBomberSprite);
    auto enemyEntity = enemy.getEntity();
    _enemies.emplace(enemy_uuid, enemyEntity);
    return _enemies.at(enemy_uuid);
}

sf::Vector2f Game::getPlayerNormalizedDirection() {
    sf::Vector2f direction(0, 0);

    if (_window.isFocused()) {
        if (sf::Keyboard::isKeyPressed(_controlSystem.getBoundKey(RealEngine::Action::Up))) {
            direction.y = -1;
        }
        if (sf::Keyboard::isKeyPressed(_controlSystem.getBoundKey(RealEngine::Action::Down))) {
            direction.y = 1;
        }
        if (sf::Keyboard::isKeyPressed(_controlSystem.getBoundKey(RealEngine::Action::Left))) {
            direction.x = -1;
        }
        if (sf::Keyboard::isKeyPressed(_controlSystem.getBoundKey(RealEngine::Action::Right))) {
            direction.x = 1;
        }
    }
    return direction;
}
}  // namespace rtype
