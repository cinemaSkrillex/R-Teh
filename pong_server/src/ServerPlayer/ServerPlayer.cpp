/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Player
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

ServerPlayer::ServerPlayer(/* args */) {}

ServerPlayer::ServerPlayer(long int uuid, long int timestamp,
                           std::shared_ptr<RealEngine::Entity> _player_entity,
                           RealEngine::Registry*               registry)
    : _uuid(uuid), _entity(_player_entity), _registry(registry), _last_update(timestamp) {}

ServerPlayer::~ServerPlayer() {}
