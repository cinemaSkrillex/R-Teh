/*
** EPITECH PROJECT, 2024
** B-CPP-500-MPL-5-2-bsrtype-yann.malaret
** File description:
** Entity.hpp
*/

#pragma once

#include <cstddef>

class Entity {
public:
    explicit Entity(std::size_t id) : _id{id} {};

    operator std::size_t() const { return _id; }

private:
    std::size_t _id;
};
