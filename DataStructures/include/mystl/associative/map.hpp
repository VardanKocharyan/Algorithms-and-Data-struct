#pragma once

#include "rb_tree.hpp"

namespace mystl {

template <typename Pair>
struct Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const noexcept { return x.first; }
};

template <typename Key, typename T, typename Compare = std::less<Key>, 
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class map {
private:
    using value_pair = std::pair<const Key, T>;
    using tree_type  = rb_tree<Key, value_pair, Select1st<value_pair>, Compare, Allocator>;
    tree_type m_tree;

public:
    using key_type       = Key;
    using mapped_type    = T;
    using value_type     = value_pair;
    using key_compare    = Compare;
    using allocator_type = Allocator;
    using size_type      = std::size_t;

    using iterator               = typename tree_type::iterator;
    using const_iterator         = typename tree_type::const_iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    map() = default;
    explicit map(const Compare& comp, const Allocator& alloc = Allocator()) : m_tree(comp, alloc) {}

    map(std::initializer_list<value_type> init) {
        for (const auto& item : init) m_tree.emplace_unique(item);
    }

    iterator begin() noexcept { return m_tree.begin(); }
    const_iterator begin() const noexcept { return m_tree.begin(); }
    iterator end() noexcept { return m_tree.end(); }
    const_iterator end() const noexcept { return m_tree.end(); }

    [[nodiscard]] bool empty() const noexcept { return m_tree.empty(); }
    size_type size() const noexcept { return m_tree.size(); }
    void clear() noexcept { m_tree.clear(); }

    T& operator[](const key_type& key) {
        return try_emplace(key).first->second;
    }

    T& operator[](key_type&& key) {
        return try_emplace(std::move(key)).first->second;
    }

    T& at(const key_type& key) {
        iterator it = find(key);
        if (it == end()) throw std::out_of_range("mystl::map::at: key not found");
        return it->second;
    }

    const T& at(const key_type& key) const {
        const_iterator it = find(key);
        if (it == end()) throw std::out_of_range("mystl::map::at: key not found");
        return it->second;
    }

    std::pair<iterator, bool> insert(const value_type& value) { return m_tree.emplace_unique(value); }
    std::pair<iterator, bool> insert(value_type&& value) { return m_tree.emplace_unique(std::move(value)); }

    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        return m_tree.emplace_unique(std::forward<Args>(args)...);
    }

    // Modern C++17 try_emplace
    template <typename... Args>
    std::pair<iterator, bool> try_emplace(const key_type& k, Args&&... args) {
        iterator it = find(k);
        if (it != end()) return {it, false};
        return m_tree.emplace_unique(std::piecewise_construct,
                                     std::forward_as_tuple(k),
                                     std::forward_as_tuple(std::forward<Args>(args)...));
    }

    template <typename... Args>
    std::pair<iterator, bool> try_emplace(key_type&& k, Args&&... args) {
        iterator it = find(k);
        if (it != end()) return {it, false};
        return m_tree.emplace_unique(std::piecewise_construct,
                                     std::forward_as_tuple(std::move(k)),
                                     std::forward_as_tuple(std::forward<Args>(args)...));
    }

    // Modern C++17 insert_or_assign
    template <typename M>
    std::pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj) {
        auto res = try_emplace(k, std::forward<M>(obj));
        if (!res.second) res.first->second = std::forward<M>(obj);
        return res;
    }

    iterator find(const key_type& key) { return m_tree.find(key); }
    const_iterator find(const key_type& key) const { return m_tree.find(key); }
    iterator lower_bound(const key_type& key) { return m_tree.lower_bound(key); }
    iterator upper_bound(const key_type& key) { return m_tree.upper_bound(key); }
};

} // namespace mystl
