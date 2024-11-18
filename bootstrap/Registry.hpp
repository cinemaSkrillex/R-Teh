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
#include <functional>
#include <stdexcept>
#include "SparseArray.hpp"
#include "ManageEntities.hpp"
#include "Entity.hpp"

class Registry {
  public:
    // Register a component type by adding a SparseArray to the registry
    template <class Component> SparseArray<Component>& register_component() {
        std::type_index index(typeid(Component));

        if (_components_arrays.find(index) == _components_arrays.end()) {
            _components_arrays[index] = std::make_any<SparseArray<Component>>();

            // Create and store the erase function
            _erase_functions.push_back([this, index](Registry& registry, Entity const& entity) {
                auto& components =
                    std::any_cast<SparseArray<Component>&>(registry._components_arrays[index]);
                components[entity].reset();
            });
        } else {
            throw std::runtime_error("Component already registered!");
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

    // Entity management methods

    // Remove an entity from all component arrays
    void remove_entity(Entity const& entity) {
        if (!_entity_manager.is_valid(entity)) {
            throw std::invalid_argument("remove_entity: Invalid entity");
        }
        for (auto& erase_function : _erase_functions) {
            erase_function(*this, entity);
        }
    }

    Entity spawn_entity() { return _entity_manager.spawn_entity(); }

    Entity entity_from_index(std::size_t idx) {
        if (idx >= _entity_manager.size()) {
            throw std::out_of_range("entity_from_index: Entity index out of range");
        }
        return _entity_manager.entity_from_index(idx);
    }

    void kill_entity(Entity const& e) {
        if (!_entity_manager.is_valid(e)) {
            throw std::invalid_argument("kill_entity: Invalid entity");
        }
        _entity_manager.kill_entity(e);
        remove_entity(e);
    }

    template <typename Component>
    typename SparseArray<Component>::reference_type add_component(Entity const& to, Component&& c) {
        auto& components = get_components<Component>();
        components[to]   = std::forward<Component>(c);
        return components[to];
    }

    // add more than one component at once
    template <typename... Components>
    void add_components(Entity const& to, Components&&... components) {
        (add_component(to, std::forward<Components>(components)), ...);
    }

    template <typename Component, typename... Params>
    typename SparseArray<Component>::reference_type emplace_component(Entity const& to,
                                                                      Params&&... p) {
        auto& components = get_components<Component>();
        components[to].emplace(std::forward<Params>(p)...);
        return components[to];
    }

    template <typename Component> void remove_component(Entity const& from) {
        auto& components = get_components<Component>();
        components[from].reset();
    }

  private:
    std::unordered_map<std::type_index, std::any>              _components_arrays;
    std::vector<std::function<void(Registry&, Entity const&)>> _erase_functions;
    ManageEntities                                             _entity_manager;
};
