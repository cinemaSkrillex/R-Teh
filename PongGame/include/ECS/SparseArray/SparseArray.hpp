/*
** EPITECH PROJECT, 2024
** B-CPP-500-MPL-5-2-bsrtype-yann.malaret
** File description:
** SparseArray.hpp
*/

#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <vector>

namespace RealEngine {
template <typename Component>

class SparseArray {
   public:
    using value_type           = std::optional<Component>;
    using reference_type       = value_type&;
    using const_reference_type = const value_type&;
    using container_t          = std::vector<value_type>;
    using size_type            = typename container_t::size_type;
    using iterator             = typename container_t::iterator;
    using const_iterator       = typename container_t::const_iterator;

    // Constructors
    SparseArray()                             = default;
    SparseArray(const SparseArray& other)     = default;
    SparseArray(SparseArray&& other) noexcept = default;
    ~SparseArray()                            = default;

    // Assignment operators
    SparseArray& operator=(const SparseArray& other)     = default;
    SparseArray& operator=(SparseArray&& other) noexcept = default;

    // Element access
    reference_type operator[](std::size_t idx) {
        if (idx >= _data.size()) {
            _data.resize(idx + 1);
        }
        return _data[idx];
    }
    const_reference_type operator[](std::size_t idx) const { return _data[idx]; }

    iterator       begin() { return _data.begin(); }
    const_iterator begin() const { return _data.begin(); }
    const_iterator cbegin() const { return _data.cbegin(); }

    iterator       end() { return _data.end(); }
    const_iterator end() const { return _data.end(); }
    const_iterator cend() const { return _data.cend(); }

    size_type      size() const { return _data.size(); }
    reference_type insertAt(size_type pos, const Component& component);
    reference_type insertAt(size_type pos, Component&& component);
    template <class... Params>
    reference_type emplaceAt(size_type pos, Params&&... params);
    void           erase(size_type pos);
    size_type      getIndex(const value_type& component) const;

   private:
    container_t _data;
};
}  // namespace RealEngine