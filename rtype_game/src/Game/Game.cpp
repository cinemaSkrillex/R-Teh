/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Game
*/

#include "Game.hpp"

namespace rtype {

RealEngine::Entity* Game::add_player(long int player_uuid, sf::Vector2f position) {
    RealEngine::Entity player = _registry.spawn_entity();
    _registry.add_component(player, RealEngine::Position{position.x, position.y});
    _registry.add_component(player, RealEngine::Drawable{});

    _registry.add_component(player, RealEngine::SpriteComponent{_otherPlayer});

    _players.emplace(player_uuid, player);
    return &_players.at(player_uuid);
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
