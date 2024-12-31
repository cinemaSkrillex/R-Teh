/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Player
*/

#include "../../include/RtypeServer.hpp"

Player::Player(/* args */) {}

Player::Player(long int uuid, long int timestamp,
               std::shared_ptr<RealEngine::Entity> _player_entity, RealEngine::Registry* registry)
    : _uuid(uuid), _entity(_player_entity), _registry(registry), _last_update(timestamp) {}

Player::~Player() {}
