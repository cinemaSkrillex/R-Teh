/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Player
*/

#include "../include/RtypeServer.hpp"

Player::Player(/* args */) {}

Player::Player(long int uuid, RealEngine::Entity* player_entity, RealEngine::Registry* registry)
    : _uuid(uuid), _entity(player_entity), _registry(registry) {}

Player::~Player() {}
