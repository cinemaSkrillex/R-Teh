/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Player
*/

#include "../include/RtypeServer.hpp"

Player::Player(/* args */) {}

Player::Player(long int uuid, sf::Vector2f current_position)
    : _uuid(uuid), _position(current_position) {}

Player::~Player() {}
