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
    std::shared_ptr<Entity> spawnEntity();

    Entity              entityFromIndex(std::size_t idx);
    void                killEntity(Entity const& e);
    std::vector<Entity> getDeadEntities() { return _deadEntities; }
    void                clearDeadEntities() { _deadEntities.clear(); }
    bool                isValid(Entity const& e) const;

    std::size_t size() const;

   private:
    std::size_t         _nextEntity = 0;
    std::vector<Entity> _deadEntities;
};
}  // namespace RealEngine