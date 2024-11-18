/*
** EPITECH PROJECT, 2024
** B-CPP-500-MPL-5-2-bsrtype-yann.malaret
** File description:
** SparseArray.hpp
*/

#pragma once

#include <cstddef>
#include <vector>
#include <optional>
#include <memory>

template <typename Component>

class SparseArray {
  public:
    using value_type           = std::optional<Component>; // Optional component type
    using reference_type       = value_type&;
    using const_reference_type = const value_type&;
    using container_t    = std::vector<value_type>; // Optionally add your allocator template here.
    using size_type      = typename container_t::size_type;
    using iterator       = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    // Constructors
    SparseArray()                             = default;
    SparseArray(const SparseArray& other)     = default;
    SparseArray(SparseArray&& other) noexcept = default;
    ~SparseArray()                            = default;

    // Assignment operators
    SparseArray& operator=(const SparseArray& other)     = default;
    SparseArray& operator=(SparseArray&& other) noexcept = default;

    // Element access
    reference_type operator[](size_t idx) {
        if (idx >= _data.size()) {
            _data.resize(idx + 1);
        }
        return _data[idx];
    }
    const_reference_type operator[](size_t idx) const { return _data[idx]; }

    // Iterators
    iterator       begin() { return _data.begin(); }
    const_iterator begin() const { return _data.begin(); }
    const_iterator cbegin() const { return _data.cbegin(); }

    iterator       end() { return _data.end(); }
    const_iterator end() const { return _data.end(); }
    const_iterator cend() const { return _data.cend(); }

    // Size
    size_type size() const { return _data.size(); }

    // Insert at a specific index
    reference_type insert_at(size_type pos, const Component& component) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos] = component;
        return _data[pos];
    }
    reference_type insert_at(size_type pos, Component&& component) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos] = std::move(component);
        return _data[pos];
    }

    // Emplace at a specific index (build component in-place)
    template <class... Params> reference_type emplace_at(size_type pos, Params&&... params) {
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

    // Erase at a specific index
    void erase(size_type pos) {
        if (pos < _data.size()) {
            _data[pos].reset();
        }
    }

    // Get the index of a component
    size_type get_index(const value_type& component) const {
        for (size_type i = 0; i < _data.size(); ++i) {
            if (std::addressof(_data[i]) == std::addressof(component)) {
                return i;
            }
        }
        return static_cast<size_type>(-1); // Return an invalid index if not found
    }

  private:
    container_t _data;
};