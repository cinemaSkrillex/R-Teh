/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** SparseArray.cpp
*/

#include "../include/ECS/SparseArray/SparseArray.hpp"

template <typename Component>
typename SparseArray<Component>::reference_type
SparseArray<Component>::insert_at(size_type pos, const Component& component) {
    if (pos >= _data.size()) {
        _data.resize(pos + 1);
    }
    _data[pos] = component;
    return _data[pos];
}

template <typename Component>
typename SparseArray<Component>::reference_type
SparseArray<Component>::insert_at(size_type pos, Component&& component) {
    if (pos >= _data.size()) {
        _data.resize(pos + 1);
    }
    _data[pos] = std::move(component);
    return _data[pos];
}

template <typename Component>
template <class... Params>
typename SparseArray<Component>::reference_type
SparseArray<Component>::emplace_at(size_type pos, Params&&... params) {
    if (pos >= _data.size()) {
        _data.resize(pos + 1);
    }

    using allocator_type     = typename container_t::allocator_type;
    allocator_type allocator = _data.get_allocator();

    if (_data[pos].has_value()) {
        std::allocator_traits<allocator_type>::destroy(allocator, std::addressof(_data[pos]));
    }

    std::allocator_traits<allocator_type>::construct(allocator, std::addressof(_data[pos]),
                                                     std::forward<Params>(params)...);

    return _data[pos];
}

/*{
        if (pos < _data.size()) {
            _data[pos].reset();
        }
    }*/

template <typename Component> void SparseArray<Component>::erase(size_type pos) {
    if (pos < _data.size()) {
        _data[pos].reset();
    }
}

template <typename Component>
typename SparseArray<Component>::size_type
SparseArray<Component>::get_index(const value_type& component) const {
    for (size_type i = 0; i < _data.size(); ++i) {
        if (std::addressof(_data[i]) == std::addressof(component)) {
            return i;
        }
    }
    return static_cast<size_type>(-1); // Return an invalid index if not found
}