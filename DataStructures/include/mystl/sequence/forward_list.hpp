#pragma once 

#include <memory>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <initializer_list>
#include <type_traits>

namespace mystl {

template <
    class T,
    class Allocator = std::allocator<T>
> class forward_list {
public:
    // 1. Types first, so nested structs can use them
    using value_type      = T;
    using allocator_type  = Allocator;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer   = typename std::allocator_traits<allocator_type>::const_pointer;

private:
    // 2. Node hierarchy
    struct NodeBase {
        NodeBase* next_{nullptr};

        explicit NodeBase(NodeBase* next = nullptr) : next_(next) {}
    };

    struct Node : NodeBase {
        value_type value_;

        template <typename... Args>
        explicit Node(NodeBase* next, Args&&... args)
            : NodeBase(next), value_(std::forward<Args>(args)...) {}
    };

    using node_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;


public:
    template <bool IsConst> 
    struct forward_list_iterator {
    public:
        using iterator_category = typename std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = std::conditional_t<IsConst, const T*, T*>;
        using reference         = std::conditional_t<IsConst, const T&, T&>;

        using node_ptr = std::conditional_t<IsConst, const Node*, Node*>;

    private:
        NodeBase* ptr_{nullptr};

        friend class forward_list;
        
    public:
        forward_list_iterator() = default;
        explicit forward_list_iterator(NodeBase* ptr) noexcept : ptr_(ptr) {}
        template <bool OtherConst, typename = std::enable_if_t<IsConst && !OtherConst>>
        forward_list_iterator(const forward_list_iterator<OtherConst>& other) noexcept
            : ptr_(other.ptr_) {}

        reference operator*() const noexcept { return static_cast<node_ptr>(ptr_)->value_; }
        pointer operator->() const { return std::addressof(operator*()); }

        forward_list_iterator& operator++() { 
            ptr_ = ptr_->next_; 
            return *this; 
        }

        forward_list_iterator operator++(int) noexcept {
            forward_list_iterator tmp = *this;
            ptr_ = ptr_->next_;
            return tmp;
        }

        friend bool operator==(const forward_list_iterator& lhs, const forward_list_iterator& rhs) noexcept {
            return lhs.ptr_ == rhs.ptr_;
        }

        friend bool operator!=(const forward_list_iterator& lhs, const forward_list_iterator& rhs) noexcept {
            return !(lhs == rhs);
        }
    };

    using iterator       = forward_list_iterator<false>;
    using const_iterator = forward_list_iterator<true>;

    // Iterators methods
    iterator before_begin() noexcept { return iterator(&head_); }
    const_iterator before_begin() const noexcept { return const_iterator(const_cast<NodeBase*>(&head_)); }
    const_iterator cbefore_begin() const noexcept { return before_begin(); }

    iterator begin() noexcept { return iterator(head_.next_); }
    const_iterator begin() const noexcept { return const_iterator(head_.next_); }
    const_iterator cbegin() const noexcept { return begin(); }

    iterator end() noexcept { return iterator(nullptr); }
    const_iterator end() const noexcept { return const_iterator(nullptr); }
    const_iterator cend() const noexcept { return end(); }

    bool empty() const noexcept { return head_.next_ == nullptr; }
    

private:
    template <typename ... Args>
    Node* create_node( NodeBase* next, Args&&... args ) {
        Node* ptr = std::allocator_traits<node_allocator_type>::allocate(alloc_, 1);

        try {
            std::allocator_traits<node_allocator_type>::construct(
                alloc_, ptr, next, std::forward<Args>(args)...
            );
        } catch (...) {
            std::allocator_traits<node_allocator_type>::deallocate(alloc_, ptr, 1);
            throw;
        }

        return ptr;
    }

    void destroy_node( Node* node ) noexcept {
        std::allocator_traits<node_allocator_type>::destroy(alloc_, node);
        std::allocator_traits<node_allocator_type>::deallocate(alloc_, node, 1);
    }

public:
    forward_list() noexcept = default;

    forward_list(const forward_list& other) : alloc_(other.alloc_) {
        insert_after(cbefore_begin(), other.begin(), other.end());
    }

    forward_list(forward_list&& other) noexcept 
        : alloc_(std::move(other.alloc_)) 
    {
        head_.next_ = other.head_.next_;
        other.head_.next_ = nullptr;
    }

    forward_list& operator=(forward_list&& other) noexcept {
        if (this != &other) {
            clear();
            alloc_ = std::move(other.alloc_);
            head_.next_ = other.head_.next_;
            other.head = nullptr;
        }
        return *this;
    }

    forward_list& operator=(const forward_list& other) {
        if (this != &other) {
            clear();
            alloc_ = other.alloc_;
            insert_after(cbefore_begin(), other.begin(), other.end());
        }
        return *this;
    }

    ~forward_list() {
        clear();
    }

    reference front() {
        return static_cast<Node*>(head_.next_)->value_;
    }

    const_reference front() const {
        return static_cast<const Node*>(head_.next_)->value_;
    }

    template< class... Args >
    iterator emplace_after( const_iterator pos, Args&&... args ) {
        NodeBase* curr = pos.ptr_;

        Node* new_node = create_node(curr->next_, std::forward<Args>(args)...);
        
        curr->next_ = new_node;
        
        return iterator(new_node);
    }

    iterator insert_after( const_iterator pos, const T& value ) { return emplace_after(pos, value); }

    iterator insert_after( const_iterator pos, T&& value ) { return emplace_after(pos, std::move(value)); }

    iterator insert_after(const_iterator pos, size_type count, const T& value) {
        if (!count) return iteratort(pos.ptr_);

        NodeBase tmp_head;
        NodeBase* tail = &tmp_head;

        try {
            for (size_type i{}; i < count; ++i) {
                tail->next_ = create_node(nullptr, value);
                tail = tail->next_;
            }
        } catch (...) {
            NodeBase* curr = tmp_head.next_;
            while (curr) {
                NodeBase* node = curr->next_;
                destroy_node(static_cast<Node*>(curr));
                curr = node;
            }
            throw;
        }

        NodeBase* pos_ptr = pos.ptr_;
        tail->next = pos_ptr->next_;
        pos_ptr->next = tmp_head.next;

        return iterator(tail);
    }

    template <class InputIt>
    iterator insert_after(const_iterator pos, InputIt first, InputIt last) {
        if (first == last) return iterator(pos.ptr_);

        NodeBase tmp_head;
        NodeBase* tail = &tmp_head;

        try {
            while (first != last) {
                tail->next_ = create_node(nullptr, *first);
                tail = tail->next_;
                ++first;
            }
        } catch (...) {
            NodeBase* curr = tmp_head.next_;
            while (curr) {
                NodeBase* node = curr->next_;
                destroy_node(static_cast<Node*>(curr));
                curr = node;
            }
            throw;
        }

        NodeBase* pos_ptr = pos.ptr_;
        tail->next_ = pos_ptr->next_;
        pos_ptr->next_ = tmp_head.next_;

        return iterator(tail);
    }

    iterator insert_after(const_iterator pos, std::initializer_list<T> ilist) {
        return insert_after(pos, ilist.begin(), ilist.end());
    }

    iterator erase_after( const_iterator pos ) noexcept {
        NodeBase* curr = pos.ptr_;
        if (!curr || !curr->next_) return end();

        NodeBase* node_to_delete = curr->next_;
        curr->next_ = node_to_delete->next_;
        
        destroy_node(static_cast<Node*>(node_to_delete));

        return iterator(curr->next_);
    }

    void push_front( const T& value ) {
        insert_after(cbefore_begin(), value);
    }

    void push_front( T&& value ) {
        insert_after(cbefore_begin(), std::move(value));
    }

    void pop_front() {
        erase_after(cbefore_begin());
    }

    void clear() noexcept {
        while (!empty()) {
            pop_front();
        }
    }

    void reverse() noexcept {
        NodeBase* curr = head_.next_;
        NodeBase* prev = nullptr;

        while (curr) {
            NodeBase* node = curr->next_;
            curr->next_ = prev;
            prev = curr;
            curr = node;
        }

        head_.next_ = prev;
    }

    template <typename Compare = std::less<T>>
    void merge(forward_list& other, Compare comp = Compare()) {
        if (this == &other || other.empty()) {
            return;
        }

        NodeBase* curr = &head_;
        NodeBase* l = head_.next_;
        NodeBase* r = other.head_.next_;

        while (l && r) {
            if (comp(static_cast<Node*>(r)->value_, static_cast<Node*>(l)->value_)) {
                curr->next_ = r;
                r = r->next_;
            } else {
                curr->next_ = l;
                l = l->next_;
            }
            curr = curr->next_;
        }

        curr->next_ = l ? l : r;
        other.head_.next_ = nullptr; 
    }

    void merge(forward_list&& other) {
        merge(other);
    }

    template <typename Compare>
    void merge(forward_list&& other, Compare comp) {
        merge(other, comp);
    }

private:
    static NodeBase* get_middle(NodeBase* head) noexcept {
        if (!head) return head;
        NodeBase* slow = head;
        NodeBase* fast = head->next_;
        
        while (fast && fast->next_) {
            slow = slow->next_;
            fast = fast->next_->next_;
        }
        return slow;
    }

public:
    template <typename Compare = std::less<T>>
    void sort(Compare comp = Compare()) {
        if (!head_.next_ || !head_.next_->next_) {
            return;
        }

        NodeBase* mid = get_middle(head_.next_);
        
        forward_list second_half;
        second_half.head_.next_ = mid->next_;
        mid->next_ = nullptr;

        this->sort(comp);
        second_half.sort(comp);

        this->merge(second_half, comp);
    }

    void sort() {
        sort(std::less<T>());
    }

private:
    NodeBase head_;
    [[no_unique_address]] node_allocator_type alloc_{};
};


} // namespace mystl




