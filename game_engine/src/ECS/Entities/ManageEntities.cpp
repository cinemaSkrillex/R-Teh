/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ManageEntities.cpp
*/

#include "../include/ECS/Entities/ManageEntities.hpp"
#include <iostream>

namespace RealEngine {

std::shared_ptr<Entity> ManageEntities::spawn_entity() {
    if (!_dead_entities.empty()) {
        Entity entity = _dead_entities.back();
        _dead_entities.pop_back();
        return std::make_shared<Entity>(entity);
    } else {
        return std::make_shared<Entity>(Entity{_next_entity++});
    }
}

// we don't check for the validity of the index here, because we do it already in the Registry.

Entity ManageEntities::entity_from_index(std::size_t idx) { return Entity{idx}; }

void ManageEntities::kill_entity(Entity const& e) { _dead_entities.push_back(e); }

bool ManageEntities::is_valid(Entity const& e) const {
    // std::cout << "e: " << e << " next_entity: " << _next_entity << std::endl;
    // std::cout << "dead_entities: ";
    // for (auto& dead : _dead_entities) {
    //     std::cout << dead << " ";
    // }
    // std::cout << "end" << std::endl;
    return e < _next_entity &&
           std::find(_dead_entities.begin(), _dead_entities.end(), e) == _dead_entities.end();
}

std::size_t ManageEntities::size() const { return _next_entity; }
}  // namespace RealEngine