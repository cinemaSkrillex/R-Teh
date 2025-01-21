/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Registry.cpp
*/

#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {

void Registry::removeEntity(Entity const& entity) {
    if (!_entityManager.isValid(entity)) {
        throw std::invalid_argument("removeEntity: Invalid entity");
    }
    for (auto& erase_function : _eraseFunctions) {
        erase_function(*this, entity);
    }
    _entityManager.killEntity(entity);
}

std::shared_ptr<Entity> Registry::spawnEntity() { return _entityManager.spawnEntity(); }

std::shared_ptr<Entity> Registry::entityFromIndex(std::size_t idx) {
    if (idx >= _entityManager.size()) {
        throw std::out_of_range("entityFromIndex: Entity index out of range");
    }
    RealEngine::Entity e      = _entityManager.entityFromIndex(idx);
    auto               entity = std::make_shared<RealEngine::Entity>(e);
    if (!entity) {
        throw std::runtime_error("entityFromIndex: Invalid entity");
    }
    return entity;
}

void Registry::killEntity(Entity const& e) {
    if (!_entityManager.isValid(e)) {
        throw std::invalid_argument("killEntity: Invalid entity");
    }

    removeEntity(e);
}

void Registry::run_systems(float deltaTime) {
    for (auto& system : _systems) {
        system(deltaTime);
    }
}

bool Registry::isValid(Entity const& e) const { return _entityManager.isValid(e); }

}  // namespace RealEngine
