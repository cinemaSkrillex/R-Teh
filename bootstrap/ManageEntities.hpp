/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ManageEntities.hpp
*/

#pragma once
#include <vector>
#include "Entity.hpp"

class ManageEntities {
  public:
    Entity spawn_entity() {
        if (!_dead_entities.empty()) {
            Entity entity = _dead_entities.back();
            _dead_entities.pop_back();
            return entity;
        } else {
            return Entity{_next_entity++};
        }
    }

    Entity entity_from_index(std::size_t idx) { return Entity{idx}; }

    void kill_entity(Entity const& e) { _dead_entities.push_back(e); }

  private:
    std::size_t         _next_entity = 0;
    std::vector<Entity> _dead_entities;
};