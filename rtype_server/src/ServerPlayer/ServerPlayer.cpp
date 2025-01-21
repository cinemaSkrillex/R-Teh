/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Player
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

ServerPlayer::ServerPlayer(/* args */) {}

ServerPlayer::ServerPlayer(long int uuid, long int timestamp,
                           std::shared_ptr<RealEngine::Entity> _playerEntity,
                           RealEngine::Registry*               registry)
    : _uuid(uuid), _entity(_playerEntity), _registry(registry), _lastUpdate(timestamp) {}

ServerPlayer::~ServerPlayer() {}
