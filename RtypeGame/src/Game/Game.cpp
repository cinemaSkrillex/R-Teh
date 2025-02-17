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
    _players.emplace(player_uuid, new_player.getEntity());
    return _players.at(player_uuid);
}

std::shared_ptr<RealEngine::Entity> Game::add_mob(long int enemy_uuid, sf::Vector2f position) {
    // // for now it adds a EyeBomber, maybe do a switch case to add different mobs
    auto entity = _registry.spawnEntity();
    _entities.emplace(enemy_uuid, EntityInfo{entity, RTypeProtocol::EntityType::OTHER_ENTITY});
    return _entities.at(enemy_uuid).entity;
}

sf::IntRect Game::getPlayerNormalizedDirection() {
    sf::IntRect direction(0, 0, 0, 0);
    auto*       player_sprite = _registry.getComponent<RealEngine::SpriteSheet>(_playerEntity);
    auto*       velocity      = _registry.getComponent<RealEngine::Velocity>(_playerEntity);

    if (player_sprite && _window.isFocused() && velocity) {
        if (_controlSystem.isActionPressed(RealEngine::Action::Left)) {
            direction.left = 1;
        }
        if (_controlSystem.isActionPressed(RealEngine::Action::Right)) {
            direction.top = 1;
        }
        if (_controlSystem.isActionPressed(RealEngine::Action::Up)) {
            direction.width            = 1;
            player_sprite->spriteIndex = "up";
        }
        if (_controlSystem.isActionPressed(RealEngine::Action::Down)) {
            direction.height           = 1;
            player_sprite->spriteIndex = "down";
        }
        if (direction.width == direction.height) {
            player_sprite->spriteIndex = "idle";
        }

        if (direction.left == 1 && direction.top == 1) {
            velocity->vx = 0;
        }
        if (direction.width == 1 && direction.height == 1) {
            velocity->vy = 0;
        }
    }
    return direction;
}
}  // namespace rtype
