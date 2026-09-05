#pragma once

#include "rb_tree.hpp"

namespace mystl {

template <typename T>
struct Identity {
    const T& operator()(const T& x) const noexcept { return x; }
};

template <typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
class set {
private:
    using tree_type = rb_tree<Key, Key, Identity<Key>, Compare, Allocator>;
    tree_type m_tree;

public:
    using key_type       = Key;
    using value_type     = Key;
    using key_compare    = Compare;
    using value_compare  = Compare;
    using allocator_type = Allocator;
    using size_type      = std::size_t;

    using iterator               = typename tree_type::const_iterator;
    using const_iterator         = typename tree_type::const_iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    set() = default;
    explicit set(const Compare& comp, const Allocator& alloc = Allocator()) : m_tree(comp, alloc) {}

    set(std::initializer_list<value_type> init) {
        for (const auto& item : init) m_tree.emplace_unique(item);
    }

    iterator begin() const noexcept { return m_tree.begin(); }
    iterator end() const noexcept { return m_tree.end(); }
    reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
    reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }

    [[nodiscard]] bool empty() const noexcept { return m_tree.empty(); }
    size_type size() const noexcept { return m_tree.size(); }
    void clear() noexcept { m_tree.clear(); }

    std::pair<iterator, bool> insert(const value_type& value) { return m_tree.emplace_unique(value); }
    std::pair<iterator, bool> insert(value_type&& value) { return m_tree.emplace_unique(std::move(value)); }

    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        return m_tree.emplace_unique(std::forward<Args>(args)...);
    }

    iterator find(const key_type& key) const { return m_tree.find(key); }
    size_type count(const key_type& key) const { return find(key) != end() ? 1 : 0; }
    iterator lower_bound(const key_type& key) const { return m_tree.lower_bound(key); }
    iterator upper_bound(const key_type& key) const { return m_tree.upper_bound(key); }
};

} // namespace mystl
