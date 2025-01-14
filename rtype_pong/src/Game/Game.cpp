/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Game
*/

#include "Game/Game.hpp"

namespace rtype {

std::shared_ptr<RealEngine::Entity> Game::add_player(long int player_uuid, sf::Vector2f position) {
    Player new_player(_registry, position, true);
    _player_entity = new_player.getEntity();
    return _player_entity;
}

int Game::getPlayerNormalizedDirection() {
    int direction = 0;
    // auto*       player_sprite = _registry.get_component<RealEngine::SpriteSheet>(_player_entity);
    auto* velocity = _registry.get_component<RealEngine::Velocity>(_player_entity);

    if (_window.isFocused() && velocity) {
        if (_controlSystem.isActionPressed(RealEngine::Action::Up)) {
            direction -= 1;
        }
        if (_controlSystem.isActionPressed(RealEngine::Action::Down)) {
            direction += 1;
        }
    }
    return direction;
}
}  // namespace rtype
