/*
** EPITECH PROJECT, 2024
** B-CPP-500-MPL-5-2-bsrtype-yann.malaret
** File description:
** Registry.hpp
*/

#pragma once

#include <any>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include "../Entities/Entity.hpp"
#include "../Entities/ManageEntities.hpp"
#include "../SparseArray/SparseArray.hpp"

namespace RealEngine {
class Registry {
   public:
    // Register a component type by adding a SparseArray to the registry
    template <class Component>
    SparseArray<Component>& register_component() {
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
    template <class Component>
    SparseArray<Component>& get_components() {
        std::type_index index(typeid(Component));

        if (_components_arrays.find(index) != _components_arrays.end()) {
            return std::any_cast<SparseArray<Component>&>(_components_arrays[index]);
        } else {
            throw std::runtime_error("");  // TODO: proper exception
        }
    }

    // Get the component array for a given type (const version)
    template <class Component>
    SparseArray<Component> const& get_components() const {
        std::type_index index(typeid(Component));

        if (_components_arrays.find(index) != _components_arrays.end()) {
            return std::any_cast<SparseArray<Component> const&>(_components_arrays.at(index));
        } else {
            throw std::runtime_error("Component not registered!");  // TODO: proper exception
        }
    }

    // Entity management methods

    // Remove an entity from all component arrays
    void   remove_entity(Entity const& entity);
    Entity spawn_entity();
    Entity entity_from_index(std::size_t idx);
    void   kill_entity(Entity const& e);
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

    template <typename Component>
    void remove_component(Entity const& from) {
        auto& components = get_components<Component>();
        components[from].reset();
    }

    // Add a system to the registry
    template <class... Components, typename Function>
    void add_system(Function&& f) {
        _systems.push_back([this, f = std::forward<Function>(f)](float deltaTime) {
            f(*this, deltaTime, get_components_helper<Components>(*this)...);
        });
    }

    template <class... Components, typename Function>
    void add_system(Function const& f) {
        _systems.push_back(
            [this, &f](float deltaTime) { f(*this, get_components_helper<Components>(*this)...); });
    }
    // Run all systems
    void run_systems(float deltaTime);
    template <typename Component>
    Component* get_component(Entity const& entity) {
        auto&       sparseArray = get_components<Component>();
        std::size_t index       = static_cast<std::size_t>(entity);

        if (index >= sparseArray.size() || !sparseArray[index]) {
            return nullptr;
        }
        return &(*sparseArray[index]);
    }
    // template <typename Component> SparseArray<Component>& get_sparse_array();
    template <typename... Components>
    std::vector<Entity> view() {
        std::vector<Entity> result;

        for (std::size_t i = 0; i < _entity_manager.size(); ++i) {
            Entity entity{i};
            if ((get_component<Components>(entity) && ...)) {
                result.push_back(entity);
            }
        }
        return result;
    }

   private:
    std::unordered_map<std::type_index, std::any>              _components_arrays;
    std::vector<std::function<void(float)>>                    _systems;
    std::vector<std::function<void(Registry&, Entity const&)>> _erase_functions;
    ManageEntities                                             _entity_manager;

    template <typename Component>
    static SparseArray<Component>& get_components_helper(Registry& registry) {
        return registry.get_components<Component>();
    }

    template <typename Component>
    static const SparseArray<Component>& get_components_helper(const Registry& registry) {
        return registry.get_components<Component>();
    }
};
}  // namespace RealEngine