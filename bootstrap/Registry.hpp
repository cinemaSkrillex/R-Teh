/*
** EPITECH PROJECT, 2024
** B-CPP-500-MPL-5-2-bsrtype-yann.malaret
** File description:
** Registry.hpp
*/

#pragma once

#include <unordered_map>
#include <typeindex>
#include <any>
#include <type_traits>
#include <iostream>
#include "SparseArray.hpp"

class Registry {
  public:
    // Register a component type by adding a SparseArray to the registry
    template <class Component> SparseArray<Component>& register_component() {
        std::type_index index(typeid(Component));

        if (_components_arrays.find(index) == _components_arrays.end()) {
            _components_arrays[index] = std::make_any<SparseArray<Component>>();
        } else {
            throw std::runtime_error("Component already registered!"); // TODO: proper exception
        }

        return std::any_cast<SparseArray<Component>&>(_components_arrays[index]);
    }

    // Get the component array for a given type (non-const version)
    template <class Component> SparseArray<Component>& get_components() {
        std::type_index index(typeid(Component));

        if (_components_arrays.find(index) != _components_arrays.end()) {
            return std::any_cast<SparseArray<Component>&>(_components_arrays[index]);
        } else {
            throw std::runtime_error("Component not registered!"); // TODO: proper exception
        }
    }

    // Get the component array for a given type (const version)
    template <class Component> SparseArray<Component> const& get_components() const {
        std::type_index index(typeid(Component));

        if (_components_arrays.find(index) != _components_arrays.end()) {
            return std::any_cast<SparseArray<Component> const&>(_components_arrays.at(index));
        } else {
            throw std::runtime_error("Component not registered!"); // TODO: proper exception
        }
    }

  private:
    std::unordered_map<std::type_index, std::any> _components_arrays;
};
