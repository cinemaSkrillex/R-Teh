/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ManageEntities.hpp
*/

#pragma once
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

#include "Entity.hpp"

namespace RealEngine {
class ManageEntities {
   public:
    std::shared_ptr<Entity> spawn_entity();

    Entity              entity_from_index(std::size_t idx);
    void                kill_entity(Entity const& e);
    std::vector<Entity> get_dead_entities() { return _dead_entities; }
    void                clear_dead_entities() { _dead_entities.clear(); }
    bool                is_valid(Entity const& e) const;

    std::size_t size() const;

   private:
    std::size_t         _next_entity = 0;
    std::vector<Entity> _dead_entities;
};
}  // namespace RealEngine