template <class... Containers> class zipper_iterator {
    template <class Container> using iterator_t     = decltype(std::declval<Container>().begin());
    template <class Container> using it_reference_t = typename iterator_t<Container>::reference;

  public:
    using value_type        = std::tuple<typename std::iterator_traits<iterator_t>::value_type&...>;
    using reference         = value_type;
    using pointer           = void;
    using difference_type   = size_t;
    using iterator_category = typename std::iterator_traits<iterator_t>::iterator_category;
    using iterator_tuple    = std ::tuple<iterator_t<Container>...>;
    // If we want zipper_iterator to be built by zipper only .
    friend containers ::zipper<Containers...>;
    zipper_iterator(iterator_tuple const& it_tuple, size_t max);

  public:
    zipper_iterator(zipper_iterator const& z);
    zipper_iterator operator++() {
        incr_all(_seq);
        while (!all_set(_seq) && _idx < _max) {
            incr_all(_seq);
            ++_idx;
        }
        return *this;
    }
    zipper_iterator& operator++(int) {
        zipper_iterator tmp(*this);
        ++(*this);
        return tmp;
    }
    value_type  operator*() { return to_value(_seq); }
    value_type  operator->() { return &to_value(_seq); }
    friend bool operator==(zipper_iterator const& lhs, zipper_iterator const& rhs) {
        return lhs._idx == rhs._idx;
    }
    friend bool operator!=(zipper_iterator const& lhs, zipper_iterator const& rhs) {
        return !(lhs == rhs);
    }

  private:
    // Increment every iterator at the same time . It also skips to the next
    // value if one of the pointed to std ::optional does not contains a value.
    template <size_t... Is> void incr_all(std::index_sequence<Is...>) {
        (..., (++std::get<Is>(_current)));
    }
    // check if every std :: optional are set .
    template <size_t... Is> bool all_set(std::index_sequence<Is...>) {
        return (... && std::get<Is>(_current) != std::end(std::get<Is>(_containers)));
    }
    // return a tuple of reference to components .
    template <size_t... Is> value_type to_value(std::index_sequence<Is...>) {
        return std::tie(*std::get<Is>(_current)...);
    }

  private:
    iterator_tuple            _current;
    std::tuple<Containers...> _containers;
    size_t                    _max; // compare this value to _idx to prevent infinite loop .
    size_t                    _idx;
    static constexpr std ::index_sequence_for<Containers...> _seq{};
};

template <class... Containers> class zipper {
  public:
    using iterator       = zipper_iterator<Containers...>;
    using iterator_tuple = typename iterator::iterator_tuple;
    zipper(Containers&... cs)
        : _begin(std::make_tuple(cs.begin()...)), _end(_compute_end(cs...)),
          _size(_compute_size(cs...)) {}

    iterator begin() { return iterator(_begin, _size); }
    iterator end() { return iterator(_end, _size); }

  private:
    static size_t _compute_size(Containers&... containers) {
        return std::min({containers.size()...});
    }
    static iterator_tuple _compute_end(Containers&... containers) {
        return std::make_tuple(containers.end()...);
    }

  private:
    iterator_tuple _begin;
    iterator_tuple _end;
    size_t         _size;
};

template <class... Containers> class indexed_zipper {
  public:
    using iterator       = indexed_zipper_iterator<Containers...>;
    using iterator_tuple = typename iterator::iterator_tuple;

    indexed_zipper(Containers&... cs)
        : _begin(std::make_tuple(cs.begin()...)), _end(_compute_end(cs...)),
          _size(_compute_size(cs...)) {}

    iterator begin() { return iterator(_begin, _size, 0); }
    iterator end() { return iterator(_end, _size, _size); }

  private:
    static size_t _compute_size(Containers&... containers) {
        return std::min({containers.size()...});
    }
    static iterator_tuple _compute_end(Containers&... containers) {
        return std::make_tuple(containers.end()...);
    }

  private:
    iterator_tuple _begin;
    iterator_tuple _end;
    size_t         _size;
};