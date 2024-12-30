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

    // _registry.add_component(player, RealEngine::Interpolation{{0, 0}, {0, 0}, 0.f, 1.f, false});
    _registry.add_component(
        player, RealEngine::Interpolation{
                    {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});

    _registry.add_component(player, RealEngine::SpriteComponent{_otherPlayer});

    _players.emplace(player_uuid, player);
    std::cout << "Player added with uuid: " << player_uuid << std::endl;
    return _players.at(player_uuid);
}

std::shared_ptr<RealEngine::Entity> Game::add_mob(long int enemy_uuid, sf::Vector2f position) {
    // // for now it adds a EyeBomber, maybe do a switch case to add different mobs
    // auto enemy       = EyeBomber(_registry, position, _eyeBomberSprite);
    // auto enemyEntity = enemy.getEntity();
    // _enemies.emplace(enemy_uuid, enemyEntity);
    // return _enemies.at(enemy_uuid);
    auto entity = _registry.spawn_entity();
    _entities.emplace(enemy_uuid, entity);
    return _entities.at(enemy_uuid);
}

sf::IntRect Game::getPlayerNormalizedDirection() {
    sf::IntRect direction(0, 0, 0, 0);

    if (_window.isFocused()) {
        if (_controlSystem.isActionPressed(RealEngine::Action::Left)) {
            direction.left = 1;
        }
        if (_controlSystem.isActionPressed(RealEngine::Action::Right)) {
            direction.top = 1;
        }
        if (_controlSystem.isActionPressed(RealEngine::Action::Up)) {
            direction.width = 1;
        }
        if (_controlSystem.isActionPressed(RealEngine::Action::Down)) {
            direction.height = 1;
        }
    }
    return direction;
}
}  // namespace rtype
