#pragma once

#include <memory>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <initializer_list>
#include <type_traits>
#include <iterator>

namespace mystl {

template <
    class T,
    class Allocator = std::allocator<T>
> class list {
private:
    struct ListNodeBase {
        ListNodeBase* prev_{nullptr};
        ListNodeBase* next_{nullptr};
    };

    struct ListNode : ListNodeBase {
        T value;

        template <typename... Args>
        explicit ListNode(Args&&... args)
            : ListNodeBase{}, value(std::forward<Args>(args)...) {}
    };

public:
    using value_type          = T;
    using allocator_type      = Allocator;
    using size_type           = std::size_t;
    using difference_type     = std::ptrdiff_t;
    using reference           = value_type&;
    using const_reference     = const value_type&;
    using pointer             = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer       = typename std::allocator_traits<allocator_type>::const_pointer;
    using node_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<ListNode>;

private:
    template <typename... Args>
    ListNode* create_node(Args&&... args) {
        ListNode* node = std::allocator_traits<node_allocator_type>::allocate(m_node_alloc, 1);

        try {
            std::allocator_traits<node_allocator_type>::construct(
                m_node_alloc, node, std::forward<Args>(args)...
            );
        } catch (...) {
            std::allocator_traits<node_allocator_type>::deallocate(m_node_alloc, node, 1);
            throw;
        }

        return node;
    }

    void destroy_node(ListNode* node) noexcept {
        std::allocator_traits<node_allocator_type>::destroy(m_node_alloc, node);
        std::allocator_traits<node_allocator_type>::deallocate(m_node_alloc, node, 1);
    }

public:
    template <bool IsConst>
    struct list_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = std::conditional_t<IsConst, const T*, T*>;
        using reference         = std::conditional_t<IsConst, const T&, T&>;

        using list_node_ptr = std::conditional_t<IsConst, const ListNode*, ListNode*>;
        using node_base_ptr = std::conditional_t<IsConst, const ListNodeBase*, ListNodeBase*>;

        list_iterator() = default;
        explicit list_iterator(node_base_ptr ptr) noexcept : list_ptr(ptr) {}

        // Implicit conversion: iterator -> const_iterator
        template <bool OtherConst, typename = std::enable_if_t<IsConst && !OtherConst>>
        list_iterator(const list_iterator<OtherConst>& other) noexcept 
            : list_ptr(other.list_ptr) {}

        reference operator*() const noexcept { 
            return static_cast<list_node_ptr>(list_ptr)->value; 
        }

        pointer operator->() const noexcept { 
            return std::addressof(operator*()); 
        }

        list_iterator& operator++() noexcept {
            list_ptr = list_ptr->next_;
            return *this;
        }

        list_iterator operator++(int) noexcept {
            list_iterator tmp = *this;
            list_ptr = list_ptr->next_;
            return tmp;
        }

        list_iterator& operator--() noexcept {
            list_ptr = list_ptr->prev_;
            return *this;
        }

        list_iterator operator--(int) noexcept {
            list_iterator tmp = *this;
            list_ptr = list_ptr->prev_;
            return tmp;
        }

        friend bool operator==(const list_iterator& lhs, const list_iterator& rhs) noexcept {
            return lhs.list_ptr == rhs.list_ptr;
        }

        friend bool operator!=(const list_iterator& lhs, const list_iterator& rhs) noexcept {
            return !(lhs == rhs);
        }

    private:
        node_base_ptr list_ptr{nullptr};

        template <bool OtherConst>
        friend struct list_iterator;

        friend class list;
    };

    using iterator       = list_iterator<false>;
    using const_iterator = list_iterator<true>;

    iterator begin() noexcept { return iterator(m_sentinel.next_); }
    const_iterator begin() const noexcept { return const_iterator(m_sentinel.next_); }
    const_iterator cbegin() const noexcept { return begin(); }

    iterator end() noexcept { return iterator(&m_sentinel); }
    const_iterator end() const noexcept { return const_iterator(&m_sentinel); }
    const_iterator cend() const noexcept { return end(); }

private:
    void swap(list& other) noexcept {
        using std::swap;
    
        swap(m_size, other.m_size);

        swap(m_sentinel.next_, other.m_sentinel.next_);
        swap(m_sentinel.prev_, other.m_sentinel.prev_);

        if (m_size > 0) {
            m_sentinel.next_->prev_ = &m_sentinel;
            m_sentinel.prev_->next_ = &m_sentinel;
        } else {
            m_sentinel.next_ = &m_sentinel;
            m_sentinel.prev_ = &m_sentinel;
        }

        if (other.m_size > 0) {
            other.m_sentinel.next_->prev_ = &other.m_sentinel;
            other.m_sentinel.prev_->next_ = &other.m_sentinel;
        } else {
            other.m_sentinel.next_ = &other.m_sentinel;
            other.m_sentinel.prev_ = &other.m_sentinel;
        }
    }
public:
    list() noexcept {
        m_sentinel.prev_ = &m_sentinel;
        m_sentinel.next_ = &m_sentinel;
        m_size = 0;
    }

    list(const list& other) : list() {
        for (const auto& item : other) {
            push_back(item);
        }
    }

    list(list&& other) noexcept : list() {
        swap(other);
    }

    list& operator=(const list& other) {
        if (this != &other) {
            list temp(other); 
            swap(temp);
        }
        return *this;
    }

    list& operator=(list&& other) noexcept {
        if (this != &other) {
            clear();
            swap(other);
        }
        return *this;
    }

    ~list() {
        clear();
    }

    template< class... Args >
    iterator emplace( const_iterator pos, Args&&... args ) {
        ListNodeBase* curr = const_cast<ListNodeBase*>(pos.list_ptr);
        ListNode* new_node = create_node(std::forward<Args>(args)...);

        new_node->prev_ = curr->prev_;
        new_node->next_ = curr;
        curr->prev_->next_ = new_node;
        curr->prev_ = new_node;

        ++m_size;
        return iterator(static_cast<ListNodeBase*>(new_node));
    }

    iterator erase( iterator pos ) noexcept {
        ListNodeBase* curr = pos.list_ptr;
        ListNodeBase* new_pos = curr->next_;

        curr->prev_->next_ = curr->next_;
        curr->next_->prev_ = curr->prev_;

        destroy_node(static_cast<ListNode*>(curr));
        --m_size;

        return iterator(new_pos);
    }

    iterator erase( const_iterator pos ) noexcept {
        ListNodeBase* curr = const_cast<ListNodeBase*>(pos.list_ptr);
        ListNodeBase* new_pos = curr->next_;

        curr->prev_->next_ = curr->next_;
        curr->next_->prev_ = curr->prev;
        
        destroy_node(static_cast<ListNode*>(curr));
        --m_size;

        return iterator(new_pos);
    }

    iterator erase( iterator first, iterator last ) noexcept {
        while (first != last) {
            first = erase(first);
        }
        return first;
    }

    iterator erase( const_iterator first, const_iterator last ) {
        while (first != last) {
            first = erase(first);
        }
        return iterator(const_cast<ListNodeBase*>(last.list_ptr));
    }

    iterator insert(const_iterator pos, const T& value) {
        return emplace(pos, value);
    }

    iterator insert(const_iterator pos, T&& value) {
        return emplace(pos, std::move(value));
    }

    iterator insert(const_iterator pos, size_type count, const T& value) {
        if (count == 0) {
            return iterator(const_cast<ListNodeBase*>(pos.list_ptr));
        }
    
        iterator first_inserted = emplace(pos, value);
        for (size_type i = 1; i < count; ++i) {
            emplace(pos, value);
        }
        return first_inserted;
    }

    template <class InputIt, typename = std::enable_if_t<!std::is_integral_v<InputIt>>>
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        if (first == last) {
            return iterator(const_cast<ListNodeBase*>(pos.list_ptr));
        }

        iterator first_inserted = emplace(pos, *first);
        ++first;
        for (; first != last; ++first) {
            emplace(pos, *first);
        }
        return first_inserted;
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
        return insert(pos, ilist.begin(), ilist.end());
    }

    void push_back( const T& value ) {
        emplace(cend(), value);
    }

    void push_back( T&& value ) {
        emplace(cend(), std::move(value));
    }

    void push_front( const T& value ) {
        emplace(cbegin(), value);
    }

    void push_front( T&& value ) {
        emplace(cbegin(), std::move(value));
    }

    template< class... Args >
    reference emplace_back( Args&&... args ) {
        return *(emplace(cend(), std::forward<Args>(args)...));
    }

    template< class... Args >
    reference emplace_front( Args&&... args ) {
        return *(emplace(cbegin(), std::forward<Args>(args)...));
    }

    void pop_back() {
        erase(std::prev(end()));
    }

    void pop_front() {
        erase(begin());
    }

    void clear() {
        erase(begin(), end());
    }

    [[nodiscard]] size_type size() const noexcept { return m_size; }
    [[nodiscard]] bool empty() const noexcept { return m_size == 0; }

    void splice(const_iterator pos, list& other) noexcept {
        if (other.empty() || this == &other) return;

        ListNodeBase* pos_node = const_cast<ListNodeBase*>(pos.list_ptr);
        ListNodeBase* first    = other.m_sentinel.next_;
        ListNodeBase* last     = other.m_sentinel.prev_;
    
        first->prev_           = pos_node->prev_;
        pos_node->prev_->next_ = first;

        last->next_            = pos_node;
        pos_node->prev_        = last;

        m_size += other.m_size;

        other.m_sentinel.next_ = &other.m_sentinel;
        other.m_sentinel.prev_ = &other.m_sentinel;
        other.m_size           = 0;
    }

    void splice(const_iterator pos, list& other, const_iterator it) noexcept {
        ListNodeBase* pos_node = const_cast<ListNodeBase*>(pos.list_ptr);
        ListNodeBase* target   = const_cast<ListNodeBase*>(it.list_ptr);

        if (pos_node == target || pos_node == target->next_) return;

        target->prev_->next_ = target->next_;
        target->next_->prev_ = target->prev_;

        target->prev_          = pos_node->prev_;
        target->next_          = pos_node;
        pos_node->prev_->next_ = target;
        pos_node->prev_        = target;

        if (this != &other) {
            ++m_size;
            --other.m_size;
        }
    }

    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last) noexcept {
        if (first == last) return;

        ListNodeBase* pos_node   = const_cast<ListNodeBase*>(pos.list_ptr);
        ListNodeBase* first_node = const_cast<ListNodeBase*>(first.list_ptr);
        ListNodeBase* last_node  = const_cast<ListNodeBase*>(last.list_ptr)->prev_; // [first, last)-ի վերջին փաստացի node-ը

        if (pos_node == first_node || pos_node == const_cast<ListNodeBase*>(last.list_ptr)) return;

        size_type count = 0;
        if (this != &other) {
            for (auto it = first; it != last; ++it) {
                ++count;
            }
        }

        first_node->prev_->next_ = last_node->next_;
        last_node->next_->prev_  = first_node->prev_;

        first_node->prev_      = pos_node->prev_;
        last_node->next_       = pos_node;
        pos_node->prev_->next_ = first_node;
        pos_node->prev_        = last_node;

        if (this != &other) {
            m_size += count;
            other.m_size -= count;
        }
    }


    void reverse() noexcept {
       if (m_size <= 1) return;

        ListNodeBase* curr = &m_sentinel;
        do {
            std::swap(curr->next_, curr->prev_);
            curr = curr->prev_;
        } while (curr != &m_sentinel);
    }

    template <class Predicate>
    size_type remove_if(Predicate pred) {
        size_type removed_count = 0;
        auto it = begin();
        while (it != end()) {
            if (pred(*it)) {
                it = erase(it);
                ++removed_count;
            } else {
                ++it;
            }
        }
        return removed_count;
    }

    size_type remove(const T& value) {
        return remove_if([&value](const T& elem) {
            return elem == value;
        });
    }

    template <class Compare = std::less<T>>
    void merge(list& other, Compare comp = Compare{}) {
        if (this == &other || other.empty()) return;

        auto first1 = begin();
        auto last1  = end();
        auto first2 = other.begin();
        auto last2  = other.end();

        while (first1 != last1 && first2 != last2) {
            if (comp(*first2, *first1)) {
                auto next2 = std::next(first2);
                splice(first1, other, first2);
                first2 = next2;
            } else {
                ++first1;
            }
        }

        if (first2 != last2) {
            splice(last1, other);
        }
    }

private:
    ListNodeBase m_sentinel;
    size_type m_size{0};
    [[no_unique_address]] node_allocator_type m_node_alloc{};
};

} // namespace mystl
