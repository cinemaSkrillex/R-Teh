/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ManageEntities.cpp
*/

#include "../include/ECS/Entities/ManageEntities.hpp"

#include <iostream>

namespace RealEngine {

std::shared_ptr<Entity> ManageEntities::spawnEntity() {
    if (!_deadEntities.empty()) {
        Entity entity = _deadEntities.back();
        _deadEntities.pop_back();
        return std::make_shared<Entity>(entity);
    } else {
        return std::make_shared<Entity>(Entity{_nextEntity++});
    }
}

// we don't check for the validity of the index here, because we do it already in the Registry.

Entity ManageEntities::entityFromIndex(std::size_t idx) { return Entity{idx}; }

void ManageEntities::killEntity(Entity const& e) {
    if (e >= _nextEntity) {
        throw std::invalid_argument("manageEntities: killEntity: Invalid entity");
    }
    _deadEntities.push_back(e);
}

bool ManageEntities::isValid(Entity const& e) const {
    if (&e == nullptr) {
        // Handle the case where the entity is invalid.
        return false;
    }
    return e < _nextEntity &&
           std::find(_deadEntities.begin(), _deadEntities.end(), e) == _deadEntities.end();
}

std::size_t ManageEntities::size() const { return _nextEntity; }
}  // namespace RealEngine