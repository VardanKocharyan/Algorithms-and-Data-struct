#pragma once

#include <memory>
#include <utility>
#include <type_traits>
#include <initializer_list>
#include <cassert>

namespace mystl {

enum class NodeColor : bool { Red = false, Black = true };

struct NodeBase {
    NodeColor color = NodeColor::Red;
    NodeBase* parent = nullptr;
    NodeBase* left   = nullptr;
    NodeBase* right  = nullptr;

    static NodeBase* minimum(NodeBase* x) noexcept {
        while (x->left) x = x->left;
        return x;
    }

    static NodeBase* maximum(NodeBase* x) noexcept {
        while (x->right) x = x->right;
        return x;
    }

    static NodeBase* increment(NodeBase* x) noexcept {
        if (x->right) return minimum(x->right);
        NodeBase* y = x->parent;
        while (x == y->right) {
            x = y;
            y = y->parent;
        }
        if (x->right != y) x = y;
        return x;
    }

    static NodeBase* decrement(NodeBase* x) noexcept {
        if (x->color == NodeColor::Red && x->parent->parent == x)
            return x->right;
        if (x->left) return maximum(x->left);
        NodeBase* y = x->parent;
        while (x == y->left) {
            x = y;
            y = y->parent;
        }
        return y;
    }
};

template <typename Value>
struct Node : public NodeBase {
    Value value;

    template <typename... Args>
    explicit Node(Args&&... args)
        : NodeBase{}, value(std::forward<Args>(args)...) {}
};

template <typename Value, bool IsConst>
class rb_tree_iterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = Value;
    using difference_type   = std::ptrdiff_t;
    using NodePtr           = std::conditional_t<IsConst, const NodeBase*, NodeBase*>;
    using reference         = std::conditional_t<IsConst, const Value&, Value&>;
    using pointer           = std::conditional_t<IsConst, const Value*, Value*>;

    NodePtr m_node = nullptr;

    rb_tree_iterator() noexcept = default;
    explicit rb_tree_iterator(NodePtr node) noexcept : m_node(node) {}

    template <bool OtherConst, typename = std::enable_if_t<IsConst && !OtherConst>>
    rb_tree_iterator(const rb_tree_iterator<Value, OtherConst>& other) noexcept 
        : m_node(other.m_node) {}

    reference operator*() const noexcept {
        using ActualPtr = std::conditional_t<IsConst, const Node<Value>*, Node<Value>*>;
        return static_cast<ActualPtr>(m_node)->value;
    }

    pointer operator->() const noexcept { return std::addressof(operator*()); }

    rb_tree_iterator& operator++() noexcept {
        m_node = NodeBase::increment(const_cast<NodeBase*>(m_node));
        return *this;
    }
    rb_tree_iterator operator++(int) noexcept {
        auto tmp = *this; ++(*this); return tmp;
    }

    rb_tree_iterator& operator--() noexcept {
        m_node = NodeBase::decrement(const_cast<NodeBase*>(m_node));
        return *this;
    }
    rb_tree_iterator operator--(int) noexcept {
        auto tmp = *this; --(*this); return tmp;
    }

    bool operator==(const rb_tree_iterator& rhs) const noexcept { return m_node == rhs.m_node; }
    bool operator!=(const rb_tree_iterator& rhs) const noexcept { return m_node != rhs.m_node; }
};

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator = std::allocator<Value>>
class rb_tree {
public:
    using key_type       = Key;
    using value_type     = Value;
    using size_type      = std::size_t;
    using allocator_type = Allocator;

    using iterator       = rb_tree_iterator<Value, false>;
    using const_iterator = rb_tree_iterator<Value, true>;

    using NodeAlloc  = typename std::allocator_traits<Allocator>::template rebind_alloc<Node<Value>>;
    using AllocTraits = std::allocator_traits<NodeAlloc>;

private:
    [[no_unique_address]] NodeAlloc m_alloc;
    [[no_unique_address]] Compare   m_comp;
    NodeBase m_header;
    size_type m_node_count = 0;

    NodeBase*& root() noexcept { return m_header.parent; }
    NodeBase* const& root() const noexcept { return m_header.parent; }
    NodeBase*& leftmost() noexcept { return m_header.left; }
    NodeBase*& rightmost() noexcept { return m_header.right; }

    template <typename... Args>
    Node<Value>* create_node(Args&&... args) {
        Node<Value>* ptr = AllocTraits::allocate(m_alloc, 1);
        try {
            AllocTraits::construct(m_alloc, ptr, std::forward<Args>(args)...);
        } catch (...) {
            AllocTraits::deallocate(m_alloc, ptr, 1);
            throw;
        }
        return ptr;
    }

    void destroy_node(NodeBase* node) noexcept {
        Node<Value>* full_node = static_cast<Node<Value>*>(node);
        AllocTraits::destroy(m_alloc, full_node);
        AllocTraits::deallocate(m_alloc, full_node, 1);
    }

    void drop_subtree(NodeBase* x) noexcept {
        while (x != nullptr) {
            drop_subtree(x->right);
            NodeBase* y = x->left;
            destroy_node(x);
            x = y;
        }
    }

public:
    rb_tree() : rb_tree(Compare(), Allocator()) {}

    explicit rb_tree(const Compare& comp, const Allocator& alloc = Allocator())
        : m_alloc(alloc), m_comp(comp) {
        init_header();
    }

    ~rb_tree() {
        clear();
    }

    void init_header() noexcept {
        m_header.color  = NodeColor::Red;
        m_header.parent = nullptr;
        m_header.left   = &m_header;
        m_header.right  = &m_header;
        m_node_count    = 0;
    }

    void clear() noexcept {
        drop_subtree(root());
        init_header();
    }

    [[nodiscard]] bool empty() const noexcept { return m_node_count == 0; }
    size_type size() const noexcept { return m_node_count; }
    Compare key_comp() const noexcept { return m_comp; }

    iterator begin() noexcept { return iterator(m_header.left); }
    const_iterator begin() const noexcept { return const_iterator(m_header.left); }
    iterator end() noexcept { return iterator(&m_header); }
    const_iterator end() const noexcept { return const_iterator(&m_header); }

    // Perfect Forwarding Emplace
    template <typename... Args>
    std::pair<iterator, bool> emplace_unique(Args&&... args) {
        Node<Value>* z = create_node(std::forward<Args>(args)...);
        const auto& key = KeyOfValue{}(z->value);

        NodeBase* x = root();
        NodeBase* y = &m_header;
        bool comp = true;

        while (x != nullptr) {
            y = x;
            comp = m_comp(key, KeyOfValue{}(static_cast<Node<Value>*>(x)->value));
            x = comp ? x->left : x->right;
        }

        iterator j(y);
        if (comp) {
            if (j == begin()) {
                insert_node(x, y, z);
                return {iterator(z), true};
            } else {
                --j;
            }
        }

        if (m_comp(KeyOfValue{}(*j), key)) {
            insert_node(x, y, z);
            return {iterator(z), true};
        }

        destroy_node(z);
        return {j, false};
    }

    iterator lower_bound(const Key& key) {
        NodeBase* x = root();
        NodeBase* y = &m_header;
        while (x) {
            if (!m_comp(KeyOfValue{}(static_cast<Node<Value>*>(x)->value), key)) {
                y = x; x = x->left;
            } else {
                x = x->right;
            }
        }
        return iterator(y);
    }

    const_iterator lower_bound(const Key& key) const {
        return const_cast<rb_tree*>(this)->lower_bound(key);
    }

    iterator upper_bound(const Key& key) {
        NodeBase* x = root();
        NodeBase* y = &m_header;
        while (x) {
            if (m_comp(key, KeyOfValue{}(static_cast<Node<Value>*>(x)->value))) {
                y = x; x = x->left;
            } else {
                x = x->right;
            }
        }
        return iterator(y);
    }

    const_iterator upper_bound(const Key& key) const {
        return const_cast<rb_tree*>(this)->upper_bound(key);
    }

    iterator find(const Key& key) {
        iterator it = lower_bound(key);
        if (it != end() && !m_comp(key, KeyOfValue{}(*it))) return it;
        return end();
    }

    const_iterator find(const Key& key) const {
        return const_cast<rb_tree*>(this)->find(key);
    }

private:
    void insert_node(NodeBase* x, NodeBase* y, NodeBase* z) noexcept {
        z->parent = y;
        if (y == &m_header) {
            root() = z;
            leftmost() = z;
            rightmost() = z;
        } else if (m_comp(KeyOfValue{}(static_cast<Node<Value>*>(z)->value), KeyOfValue{}(static_cast<Node<Value>*>(y)->value))) {
            y->left = z;
            if (y == leftmost()) leftmost() = z;
        } else {
            y->right = z;
            if (y == rightmost()) rightmost() = z;
        }
        
        rebalance_for_insert(z);
        ++m_node_count;
    }

    void rebalance_for_insert(NodeBase* x) noexcept {
        x->color = NodeColor::Red;
        while (x != root() && x->parent->color == NodeColor::Red) {
            NodeBase* g = x->parent->parent;
            if (x->parent == g->left) {
                NodeBase* uncle = g->right;
                if (uncle && uncle->color == NodeColor::Red) {
                    x->parent->color = NodeColor::Black;
                    uncle->color = NodeColor::Black;
                    g->color = NodeColor::Red;
                    x = g;
                } else {
                    if (x == x->parent->right) {
                        x = x->parent;
                        rotate_left(x);
                    }
                    x->parent->color = NodeColor::Black;
                    g->color = NodeColor::Red;
                    rotate_right(g);
                }
            } else {
                NodeBase* uncle = g->left;
                if (uncle && uncle->color == NodeColor::Red) {
                    x->parent->color = NodeColor::Black;
                    uncle->color = NodeColor::Black;
                    g->color = NodeColor::Red;
                    x = g;
                } else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        rotate_right(x);
                    }
                    x->parent->color = NodeColor::Black;
                    g->color = NodeColor::Red;
                    rotate_left(g);
                }
            }
        }
        root()->color = NodeColor::Black;
    }

    void rotate_left(NodeBase* x) noexcept {
        NodeBase* y = x->right;
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
        if (x == root()) root() = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rotate_right(NodeBase* x) noexcept {
        NodeBase* y = x->left;
        x->left = y->right;
        if (y->right) y->right->parent = x;
        y->parent = x->parent;
        if (x == root()) root() = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x;
        x->parent = y;
    }
};

} // namespace mystl
