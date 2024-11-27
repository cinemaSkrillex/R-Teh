/*
** EPITECH PROJECT, 2024
** B-CPP-500-MPL-5-2-bsrtype-yann.malaret
** File description:
** Entity.hpp
*/

#pragma once

#include <cstddef>

namespace RealEngine {
class Registry;
class ManageEntities;

class Entity {
  public:
    operator std::size_t() const { return _id; }

  private:
    // constructor is private to prevent other classes from creating entities
    explicit Entity(std::size_t id) : _id{id} {};
    std::size_t _id;

    // Allow the registry to create entities
    friend class Registry;
    friend class ManageEntities;
};
} // namespace RealEngine