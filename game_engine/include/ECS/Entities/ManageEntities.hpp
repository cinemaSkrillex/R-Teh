/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ManageEntities.hpp
*/

#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "Entity.hpp"

namespace RealEngine {
class ManageEntities {
  public:
    Entity spawn_entity();

    Entity entity_from_index(std::size_t idx);
    void   kill_entity(Entity const& e);
    bool   is_valid(Entity const& e) const;

    std::size_t size() const;

  private:
    std::size_t         _next_entity = 0;
    std::vector<Entity> _dead_entities;
};
} // namespace RealEngine