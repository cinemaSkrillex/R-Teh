/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Registry.cpp
*/

#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {

void Registry::remove_entity(Entity const& entity) {
    if (!_entity_manager.is_valid(entity)) {
        throw std::invalid_argument("remove_entity: Invalid entity");
    }
    for (auto& erase_function : _erase_functions) {
        erase_function(*this, entity);
    }
}

Entity Registry::spawn_entity() { return _entity_manager.spawn_entity(); }

Entity Registry::entity_from_index(std::size_t idx) {
    if (idx >= _entity_manager.size()) {
        throw std::out_of_range("entity_from_index: Entity index out of range");
    }
    return _entity_manager.entity_from_index(idx);
}

void Registry::kill_entity(Entity const& e) {
    if (!_entity_manager.is_valid(e)) {
        throw std::invalid_argument("kill_entity: Invalid entity");
    }
    _entity_manager.kill_entity(e);
    remove_entity(e);
}

void Registry::run_systems(float deltaTime) {
    for (auto& system : _systems) {
        system(deltaTime);
    }
}
} // namespace RealEngine