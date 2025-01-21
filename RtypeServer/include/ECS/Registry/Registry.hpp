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
    SparseArray<Component>& registerComponent() {
        std::type_index index(typeid(Component));

        if (_componentsArrays.find(index) == _componentsArrays.end()) {
            _componentsArrays[index] = std::make_any<SparseArray<Component>>();

            _eraseFunctions.push_back([this, index](Registry& registry, Entity const& entity) {
                auto& components =
                    std::any_cast<SparseArray<Component>&>(registry._componentsArrays[index]);
                components[entity].reset();
            });
        } else {
            throw std::runtime_error("Component already registered!");
        }

        return std::any_cast<SparseArray<Component>&>(_componentsArrays[index]);
    }

    template <class Component>
    SparseArray<Component>& getComponents() {
        std::type_index index(typeid(Component));

        try {
            if (_componentsArrays.find(index) != _componentsArrays.end()) {
                return std::any_cast<SparseArray<Component>&>(_componentsArrays[index]);
            } else {
                throw std::runtime_error(std::string("getComponents: Component not registered: ") +
                                         typeid(Component).name());
            }
        } catch (const std::bad_any_cast& e) {
            throw std::runtime_error(std::string("getComponents: Bad any_cast for component: ") +
                                     typeid(Component).name() + " - " + e.what());
        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("getComponents: Exception for component: ") +
                                     typeid(Component).name() + " - " + e.what());
        }
    }

    template <class Component>
    SparseArray<Component> const& getComponents() const {
        std::type_index index(typeid(Component));

        if (_componentsArrays.find(index) != _componentsArrays.end()) {
            return std::any_cast<SparseArray<Component> const&>(_componentsArrays.at(index));
        } else {
            throw std::runtime_error("Component not registered!");
        }
    }

    void                    removeEntity(Entity const& entity);
    std::shared_ptr<Entity> spawnEntity();
    std::shared_ptr<Entity> entityFromIndex(std::size_t idx);
    bool                    isValid(Entity const& e) const;
    void                    killEntity(Entity const& e);

    template <typename Component>
    typename SparseArray<Component>::reference_type addComponent(std::shared_ptr<Entity> to,
                                                                 Component&&             c) {
        auto& components = getComponents<Component>();
        components[*to]  = std::forward<Component>(c);
        return components[*to];
    }

    template <typename Component>
    typename SparseArray<Component>::reference_type addComponent(Entity const& to, Component&& c) {
        auto& components = getComponents<Component>();
        components[to]   = std::forward<Component>(c);
        return components[to];
    }

    template <typename... Components>
    void addComponents(std::shared_ptr<Entity> to, Components&&... components) {
        (addComponent(to, std::forward<Components>(components)), ...);
    }

    template <typename Component, typename... Params>
    typename SparseArray<Component>::reference_type emplaceComponent(std::shared_ptr<Entity> to,
                                                                     Params&&... p) {
        auto& components = getComponents<Component>();
        components[*to].emplace(std::forward<Params>(p)...);
        return components[*to];
    }

    template <typename Component>
    void removeComponent(std::shared_ptr<Entity> from) {
        auto& components = getComponents<Component>();
        components[*from].reset();
    }

    template <typename Component>
    void removeComponent(Entity const& from) {
        auto& components = getComponents<Component>();
        components[from].reset();
    }

    template <class... Components, typename Function>
    void addSystem(Function&& f) {
        _systems.push_back([this, f = std::forward<Function>(f)](float deltaTime) {
            f(*this, deltaTime, getComponentsHelper<Components>(*this)...);
        });
    }

    template <class... Components, typename Function>
    void addSystem(Function const& f) {
        _systems.push_back(
            [this, &f](float deltaTime) { f(*this, getComponentsHelper<Components>(*this)...); });
    }
    void run_systems(float deltaTime);

    template <typename Component>
    Component* getComponent(std::shared_ptr<Entity> entity) {
        if (!entity) {
            return nullptr;
        }
        auto&       sparseArray = getComponents<Component>();
        std::size_t index       = static_cast<std::size_t>(*entity);

        if (index >= sparseArray.size() || !sparseArray[index]) {
            return nullptr;
        }
        return &(*sparseArray[index]);
    }

    template <typename Component>
    Component* getComponent(Entity const& entity) {
        if (!isValid(entity)) {
            return nullptr;
        }

        auto&       sparseArray = getComponents<Component>();
        std::size_t index       = static_cast<std::size_t>(entity);

        if (index >= sparseArray.size() || !sparseArray[index]) {
            return nullptr;
        }
        return &(*sparseArray[index]);
    }

    template <typename Component>
    std::vector<Component*> getComponents(std::shared_ptr<Entity> entity) {
        if (!entity) {
            return std::vector<Component*>();
        }

        auto&       sparseArray = getComponents<Component>();
        std::size_t index       = static_cast<std::size_t>(*entity);

        std::vector<Component*> components;

        if (index >= sparseArray.size() || !sparseArray[index]) {
            return components;
        }

        components.push_back(&(*sparseArray[index]));
        return components;
    }

    template <typename Component>
    std::vector<Component*> getComponents(Entity const& entity) {
        if (!isValid(entity)) {
            return std::vector<Component*>();
        }
        auto&       sparseArray = getComponents<Component>();
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

        for (std::size_t i = 0; i < _entityManager.size(); ++i) {
            Entity entity{i};
            if ((getComponent<Components>(entity) && ...)) {
                result.push_back(entity);
            }
        }
        return result;
    }

   private:
    std::unordered_map<std::type_index, std::any>              _componentsArrays;
    std::vector<std::function<void(float)>>                    _systems;
    std::vector<std::function<void(Registry&, Entity const&)>> _eraseFunctions;
    ManageEntities                                             _entityManager;

    template <typename Component>
    static SparseArray<Component>& getComponentsHelper(Registry& registry) {
        return registry.getComponents<Component>();
    }

    template <typename Component>
    static const SparseArray<Component>& getComponentsHelper(const Registry& registry) {
        return registry.getComponents<Component>();
    }
};
}  // namespace RealEngine