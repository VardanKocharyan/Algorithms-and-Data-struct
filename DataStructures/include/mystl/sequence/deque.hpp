#pragma once 

#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace mystl {

template <
    class T,
    class Allocator = std::allocator<T>
> class deque {
public:
    using value_type        = T;
    using allocator_type    = Allocator;
    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using pointer           = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer     = typename std::allocator_traits<allocator_type>::const_pointer;
    
    using data_traits        = std::allocator_traits<allocator_type>;
    using map_allocator_type = typename data_traits::template rebind_alloc<T*>;
    using map_traits         = std::allocator_traits<map_allocator_type>;

private:
    static constexpr size_type buffer_size() {
        return (sizeof(T) < 512) ? 512 / sizeof(T) : 1;
    }

    T* allocate_node() {
        return data_traits::allocate(m_allocator, buffer_size());
    }

    void deallocate_node(T* p) {
        data_traits::deallocate(m_allocator, p, buffer_size());
    }

    T** allocate_map(size_type n) {
        return map_traits::allocate(m_map_allocator, n);
    }

    void deallocate_map(T** p, size_type n) {
        map_traits::deallocate(m_map_allocator, p, n);
    }

    void create_map_and_nodes(size_type num_elements) {
        size_type num_nodes = num_elements / buffer_size() + 1;

        m_map_size = std::max(static_cast<size_type>(8), num_nodes + 2);
        m_map = allocate_map(m_map_size);
        std::fill_n(m_map, m_map_size, nullptr);

        T** nstart = m_map + (m_map_size - num_nodes) / 2;
        T** nfinish = nstart + num_nodes;

        T** cur = nstart;
        try {
            for (; cur < nfinish; ++cur) {
                *cur = allocate_node();
            }
        } catch (...) {
            for (T** p = nstart; p < cur; ++p) {
                deallocate_node(*p);
            }
            deallocate_map(m_map, m_map_size);
        
            m_map = nullptr;
            m_map_size = 0;
            throw; 
        }

        m_start.set_node(nstart);
        m_start.cur = m_start.first;
        m_finish.set_node(nfinish - 1);
        m_finish.cur = m_finish.first + (num_elements % buffer_size());
    }

    void destroy_map_and_nodes() {
        if (!m_map) return;

        for (T** chunk = m_map; chunk < m_map + m_map_size; ++chunk) {
            if (*chunk) deallocate_node(*chunk);
        }
        deallocate_map(m_map, m_map_size);
        m_map = nullptr;
        m_map_size = 0;
    }

    void push_back_aux(const T& value) {
        reserve_map_at_back();

        *(m_finish.node + 1) = allocate_node();

        try {
            data_traits::construct(m_allocator, m_finish.cur, value);
        
            m_finish.set_node(m_finish.node + 1);
            m_finish.cur = m_finish.first;
        } catch (...) {
            deallocate_node(*(m_finish.node + 1));
            throw;
        }
    }

    void reserve_map_at_back(size_type nodes_to_add = 1) {
        if (nodes_to_add + 1 > m_map_size - (m_finish.node - m_map)) {
            reallocate_map(nodes_to_add, true);
        }
    }

    void reallocate_map(size_type nodes_to_add, bool add_at_back) {
        size_type old_num_nodes = m_finish.node - m_start.node + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;
    
        T** new_nstart;
    
        if (m_map_size > 2 * new_num_nodes) {
            new_nstart = m_map + (m_map_size - new_num_nodes) / 2 
                         + (add_at_back ? 0 : nodes_to_add);
                     
            if (new_nstart < m_start.node) {
                std::copy(m_start.node, m_finish.node + 1, new_nstart);
            } else {
                std::copy_backward(m_start.node, m_finish.node + 1, new_nstart + old_num_nodes);
            }
        } 
        else {
            size_type new_map_size = m_map_size + std::max(m_map_size, nodes_to_add) + 2;
            T** new_map = allocate_map(new_map_size);
            std::fill_n(new_map, new_map_size, nullptr);
        
            new_nstart = new_map + (new_map_size - new_num_nodes) / 2 
                         + (add_at_back ? 0 : nodes_to_add);
                         
            std::copy(m_start.node, m_finish.node + 1, new_nstart);
        
            deallocate_map(m_map, m_map_size);
        
            m_map = new_map;
            m_map_size = new_map_size;
        }

        m_start.set_node(new_nstart);
        m_finish.set_node(new_nstart + old_num_nodes - 1);
    }

    void reserve_map_at_front(size_type nodes_to_add = 1) {
        if (nodes_to_add > static_cast<size_type>(m_start.node - m_map)) {
            reallocate_map(nodes_to_add, false); 
        }
    }

    void push_front_aux(const T& value) {
        reserve_map_at_front();
        
        *(m_start.node - 1) = allocate_node();
        
        try {
            m_start.set_node(m_start.node - 1);
            m_start.cur = m_start.last - 1;
            
            data_traits::construct(m_allocator, m_start.cur, value);
        } catch (...) {
            m_start.set_node(m_start.node + 1);
            m_start.cur = m_start.first;
            deallocate_node(*(m_start.node - 1));
            throw;
        }
    }

    void pop_back_aux() {
        deallocate_node(m_finish.first);
        m_finish.set_node(m_finish.node - 1);
        m_finish.cur = m_finish.last - 1;
        data_traits::destroy(m_allocator, m_finish.cur);
    }

    void pop_front_aux() {
        data_traits::destroy(m_allocator, m_start.cur);
        deallocate_node(m_start.first);
        m_start.set_node(m_start.node + 1);
        m_start.cur = m_start.first;
    }

public:
    template <bool IsConst>
    struct deque_iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = std::conditional_t<IsConst, const T*, T*>;
        using reference         = std::conditional_t<IsConst, const T&, T&>;

    private:
        void set_node(T** new_node) {
            node = new_node;
            first = *new_node;
            last = first + buffer_size();
        }

    public:
        deque_iterator() = default;
        
        deque_iterator(T* c, T** n) {
            set_node(n);
            cur = c;
        }

        template <bool B = IsConst, typename = std::enable_if_t<B>>
        deque_iterator(const deque_iterator<false>& other)
            : cur(other.cur), first(other.first), last(other.last), node(other.node) {}

        reference operator*() const { return *cur; }
        pointer operator->() const { return cur; }

        template <bool OtherConst>
        bool operator==(const deque_iterator<OtherConst>& rhs) const {
           return cur == rhs.cur;
        }

        template <bool OtherConst>
        bool operator!=(const deque_iterator<OtherConst>& rhs) const {
            return !(*this == rhs);
        }

        deque_iterator& operator++() {
            ++cur;
            if (cur == last) {
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }

        deque_iterator operator++(int) {
            deque_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        deque_iterator& operator--() {
            if (cur == first) {
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }

        deque_iterator operator--(int) {
            deque_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        deque_iterator& operator+=(difference_type n) {
            difference_type offset = (cur - first) + n;
            if (offset >= 0 && offset < static_cast<difference_type>(buffer_size())) {
                cur += n;
            } else {
                difference_type node_offset = offset > 0 
                    ? offset / static_cast<difference_type>(buffer_size())
                    : -static_cast<difference_type>((-offset - 1) / buffer_size()) - 1;
    
                set_node(node + node_offset);
                cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size()));
            }
            return *this;
        }

        deque_iterator operator+(difference_type n) const {
            deque_iterator tmp = *this;
            tmp += n;
            return tmp;
        }

        friend deque_iterator operator+(difference_type n, const deque_iterator& it) {
            return it + n;
        }

        deque_iterator& operator-=(difference_type n) {
            return *this += -n;
        }

        deque_iterator operator-(difference_type n) const {
            deque_iterator tmp = *this;
            tmp -= n;
            return tmp;
        }

        template <bool OtherConst>
        difference_type operator-(const deque_iterator<OtherConst>& rhs) const {
            if (node == rhs.node) {
                return cur - rhs.cur;
            }
            return static_cast<difference_type>(buffer_size()) * (node - rhs.node - 1)
                + (cur - first) 
                + (rhs.last - rhs.cur);
        }

        reference operator[](difference_type n) const {
            return *(*this + n);
        }

    private:
        T* cur{nullptr};
        T* first{nullptr};
        T* last{nullptr};
        T** node{nullptr};

        template <typename U, typename Alloc>
        friend class deque;

        template <bool OtherConst>
        friend struct deque_iterator;
    }; // deque_iterator

    using iterator       = deque_iterator<false>;
    using const_iterator = deque_iterator<true>;

private:
    template <typename... Args>
    iterator emplace_back_aux(Args&&... args) {
        reserve_map_at_back();
        
        *(m_finish.node + 1) = allocate_node();
        
        try {
            data_traits::construct(
                m_allocator, 
                m_finish.cur, 
                std::forward<Args>(args)...
            );
            
            m_finish.set_node(m_finish.node + 1);
            m_finish.cur = m_finish.first;
        } catch (...) {
            deallocate_node(*(m_finish.node + 1));
            throw;
        }
        
        iterator ret = m_finish;
        --ret;
        return ret;
    }

    template <typename... Args>
    iterator emplace_front_aux(Args&&... args) {
        reserve_map_at_front();
        
        *(m_start.node - 1) = allocate_node();
        
        try {
            m_start.set_node(m_start.node - 1);
            m_start.cur = m_start.last - 1;
            
            data_traits::construct(
                m_allocator, 
                m_start.cur, 
                std::forward<Args>(args)...
            );
        } catch (...) {
            m_start.set_node(m_start.node + 1);
            m_start.cur = m_start.first;
            deallocate_node(*(m_start.node - 1));
            throw;
        }
        
        return m_start;
    }

public:
    deque() {
        create_map_and_nodes(0);
    }

    explicit deque(size_type count) {
        create_map_and_nodes(count);
    }

    ~deque() {
        clear();
        if (m_start.node) {
            deallocate_node(m_start.first);
        }
        if (m_map) {
            deallocate_map(m_map, m_map_size);
        }
    }

    iterator begin() noexcept { return m_start; }
    iterator end() noexcept { return m_finish; }
    const_iterator begin() const noexcept { return m_start; }
    const_iterator end() const noexcept { return m_finish; }

    void push_back(const T& value) {
        emplace_back(value);
    }

    void push_back(T&& value) {
        emplace_back(std::move(value));
    }

    void push_front(const T& value) {
        emplace_front(value);
    }

    void push_front(T&& value) {
        emplace_front(std::move(value));
    }    

    void pop_back() {
        if (m_finish.cur != m_finish.first) {
            --m_finish.cur;
            data_traits::destroy(m_allocator, m_finish.cur);
        } else {
            pop_back_aux();
        }
    }

    void pop_front() {
        if (m_start.cur != m_start.last - 1) {
            data_traits::destroy(m_allocator, m_start.cur);
            ++m_start.cur;
        } else {
            pop_front_aux();
        }
    }

    size_type size() const noexcept {
        return static_cast<size_type>(m_finish - m_start);
    }

    bool empty() const noexcept {
        return m_finish == m_start;
    }

    reference operator[](size_type n) {
        return m_start[static_cast<difference_type>(n)];
    }

    const_reference operator[](size_type n) const {
        return m_start[static_cast<difference_type>(n)];
    }

    reference front() {
        return *m_start;
    }

    const_reference front() const {
        return *m_start;
    }

    reference back() {
        iterator tmp = m_finish;
        --tmp;
        return *tmp;
    }

    const_reference back() const {
        const_iterator tmp = m_finish;
        --tmp;
        return *tmp;
    }

    void clear() noexcept {
        for (T** node = m_start.node + 1; node < m_finish.node; ++node) {
            for (T* p = *node; p < *node + buffer_size(); ++p) {
                data_traits::destroy(m_allocator, p);
            }
            deallocate_node(*node);
        }

        if (m_start.node != m_finish.node) {
            for (T* p = m_start.cur; p < m_start.last; ++p) {
                data_traits::destroy(m_allocator, p);
            }
            for (T* p = m_finish.first; p < m_finish.cur; ++p) {
                data_traits::destroy(m_allocator, p);
            }
            deallocate_node(m_finish.first);
        } else {
            for (T* p = m_start.cur; p < m_finish.cur; ++p) {
                data_traits::destroy(m_allocator, p);
            }
        }

        m_finish = m_start;
    }

    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (m_finish.cur != m_finish.last - 1) {
            data_traits::construct(
                m_allocator, 
                m_finish.cur, 
                std::forward<Args>(args)...    
            );
            reference ref = *m_finish.cur;
            ++m_finish.cur;
            return ref;
        } else {
            return *emplace_back_aux(std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    reference emplace_front(Args&&... args) {
        if (m_start.cur != m_start.first) {
            --m_start.cur;
            data_traits::construct(
                m_allocator, 
                m_start.cur, 
                std::forward<Args>(args)...
            );
            return *m_start.cur;
        } else {
            return *emplace_front_aux(std::forward<Args>(args)...);
        }
    }

private:
    T** m_map{nullptr};
    size_type m_map_size{0};
    iterator m_start;
    iterator m_finish;
    [[no_unique_address]] allocator_type m_allocator;
    [[no_unique_address]] map_allocator_type m_map_allocator;

}; //class deque 

} //namespace mystl
