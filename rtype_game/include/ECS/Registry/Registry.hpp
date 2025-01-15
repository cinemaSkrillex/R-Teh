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
#include <memory>
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
    template <class Component>
    SparseArray<Component>& register_component() {
        std::type_index index(typeid(Component));

        if (_components_arrays.find(index) == _components_arrays.end()) {
            _components_arrays[index] = std::make_any<SparseArray<Component>>();

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

    template <class Component>
    SparseArray<Component>& get_components() {
        std::type_index index(typeid(Component));

        try {
            if (_components_arrays.find(index) != _components_arrays.end()) {
                return std::any_cast<SparseArray<Component>&>(_components_arrays[index]);
            } else {
                throw std::runtime_error(std::string("get_components: Component not registered: ") +
                                         typeid(Component).name());
            }
        } catch (const std::bad_any_cast& e) {
            throw std::runtime_error(std::string("get_components: Bad any_cast for component: ") +
                                     typeid(Component).name() + " - " + e.what());
        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("get_components: Exception for component: ") +
                                     typeid(Component).name() + " - " + e.what());
        }
    }

    template <class Component>
    SparseArray<Component> const& get_components() const {
        std::type_index index(typeid(Component));

        if (_components_arrays.find(index) != _components_arrays.end()) {
            return std::any_cast<SparseArray<Component> const&>(_components_arrays.at(index));
        } else {
            throw std::runtime_error("Component not registered!");
        }
    }

    void                    remove_entity(Entity const& entity);
    std::shared_ptr<Entity> spawn_entity();
    std::shared_ptr<Entity> entity_from_index(std::size_t idx);
    bool                    is_valid(Entity const& e) const;
    void                    kill_entity(Entity const& e);

    template <typename Component>
    typename SparseArray<Component>::reference_type add_component(std::shared_ptr<Entity> to,
                                                                  Component&&             c) {
        auto& components = get_components<Component>();
        components[*to]  = std::forward<Component>(c);
        return components[*to];
    }

    template <typename Component>
    typename SparseArray<Component>::reference_type add_component(Entity const& to, Component&& c) {
        auto& components = get_components<Component>();
        components[to]   = std::forward<Component>(c);
        return components[to];
    }

    template <typename... Components>
    void add_components(std::shared_ptr<Entity> to, Components&&... components) {
        (add_component(to, std::forward<Components>(components)), ...);
    }

    template <typename Component, typename... Params>
    typename SparseArray<Component>::reference_type emplace_component(std::shared_ptr<Entity> to,
                                                                      Params&&... p) {
        auto& components = get_components<Component>();
        components[*to].emplace(std::forward<Params>(p)...);
        return components[*to];
    }

    template <typename Component>
    void remove_component(std::shared_ptr<Entity> from) {
        auto& components = get_components<Component>();
        components[*from].reset();
    }

    template <typename Component>
    void remove_component(Entity const& from) {
        auto& components = get_components<Component>();
        components[from].reset();
    }

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
    void run_systems(float deltaTime);

    template <typename Component>
    Component* get_component(std::shared_ptr<Entity> entity) {
        if (!entity) {
            return nullptr;
        }
        auto&       sparseArray = get_components<Component>();
        std::size_t index       = static_cast<std::size_t>(*entity);

        if (index >= sparseArray.size() || !sparseArray[index]) {
            return nullptr;
        }
        return &(*sparseArray[index]);
    }

    template <typename Component>
    Component* get_component(Entity const& entity) {
        if (!is_valid(entity)) {
            return nullptr;
        }

        auto&       sparseArray = get_components<Component>();
        std::size_t index       = static_cast<std::size_t>(entity);

        if (index >= sparseArray.size() || !sparseArray[index]) {
            return nullptr;
        }
        return &(*sparseArray[index]);
    }

    template <typename Component>
    std::vector<Component*> get_components(std::shared_ptr<Entity> entity) {
        if (!entity) {
            return std::vector<Component*>();
        }

        auto&       sparseArray = get_components<Component>();
        std::size_t index       = static_cast<std::size_t>(*entity);

        std::vector<Component*> components;

        if (index >= sparseArray.size() || !sparseArray[index]) {
            return components;
        }

        components.push_back(&(*sparseArray[index]));
        return components;
    }

    template <typename Component>
    std::vector<Component*> get_components(Entity const& entity) {
        if (!is_valid(entity)) {
            return std::vector<Component*>();
        }
        auto&       sparseArray = get_components<Component>();
        std::size_t index       = static_cast<std::size_t>(entity);

        std::vector<Component*> components;

        if (index >= sparseArray.size() || !sparseArray[index]) {
            return components;
        }

        components.push_back(&(*sparseArray[index]));
        return components;
    }

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