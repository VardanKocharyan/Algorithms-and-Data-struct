#pragma once 

template <typename T>
class my_forward_list {

    private:
        //Node
        struct Node {
            T value;
            Node* next = nullptr;

            Node()  = default;
            Node(const T& elem) : value(elem) {}
            Node(T&& elem) : value(std::move(elem)) {}
            Node(const Node& r) : value(r.value) {}
            Node(Node&& r) : value(r.value), next(r.next) {
                r.next = nullptr;
            }
            Node(const T& elem, Node* next_) : value(elem), next(next_) {}
            Node(T&& elem, Node* next_) : value(std::move(elem)), next(next_) {}

            ~Node() = default;

            Node& operator++() { return next;}
        };

    public:
        //iterator
        template <typename DataType>
        struct Iterator {
            using iterator_category = std::forward_iterator_tag;
            using value_type        = DataType;
            using reference         = DataType&;
            using difference_type   = ptrdiff_t;
            using pointer           = DataType*;

            Node* current;

            Iterator(Node* current_ = nullptr) : current(current_) {}
            template <typename OtherType>
            Iterator(const Iterator<OtherType>& other) : current(other.current) {}

            constexpr reference operator*() { return current->value; }
            constexpr pointer operator->() { return &(current->value); }
            
            constexpr Iterator& operator++() const noexcept {
                current = current->next;
                return *this;
            }

            Iterator operator++(int) const noexcept {
                Iterator tmp = *this;
                current = current->next;
                return tmp;
            }

            constexpr bool operator==(const Iterator& r) const noexcept { return current == r.current; }
            constexpr bool operator!=(const Iterator& r) const noexcept { return !(*this == r); }
        };

    public:
        using value_type      = T;
        using size_type       = size_t;
        using reference       = value_type&;
        using const_reference = const value_type&;
        using pointer         = value_type*;
        using const_pointer   = const value_type*;
        using iterator        = Iterator<T>;
        using const_iterator  = Iterator<const T>;

    private:
        Node* sentinel;

    public:
//ctors
        my_forward_list() : sentinel(new Node()) {}
        explicit my_forward_list(size_type count);
        explicit my_forward_list(size_type count, const value_type& elem);
/*
        template <typename InputIt>
        my_forward_list(InputIt first, InputIt last) {}
*/
        my_forward_list(const my_forward_list<T>& other);
        my_forward_list(my_forward_list<T>&& other);
        my_forward_list(std::initializer_list<T> init);

//dtor
        ~my_forward_list() { clear(); delete sentinel; }

//operator=
        my_forward_list<T>& operator=(const my_forward_list<value_type>& other);
        my_forward_list<T>& operator=(my_forward_list<value_type>&& other);
        my_forward_list<T>& operator=( std::initializer_list<value_type> ilist );

        iterator insert_after(const_iterator pos, const T& value);
        iterator insert_after(const_iterator pos, T&& value);
        iterator insert_after(const_iterator pos, size_type count, const T& value);
        iterator insert_after(const_iterator pos, std::initializer_list<T> ilist);
        template <typename InputIt = std::input_iterator_tag>
        iterator insert_after(const_iterator pos, InputIt first, InputIt last);

        iterator erase_after(const_iterator pos);
        iterator erase_after(const_iterator first, const_iterator last);

        bool empty() {return sentinel->next == nullptr; }
        void clear();

        reference front() { return *begin(); }
        const_reference front() const { return *begin(); }


        iterator begin() noexcept { return iterator(sentinel->next); }
        const_iterator begin() const noexcept { return const_iterator(sentinel->next); }
        const_iterator cbegin() const noexcept { return const_iterator(sentinel->next); }
        iterator end() noexcept { return iterator(nullptr); }
        const_iterator end() const noexcept { return const_iterator(nullptr); }
        const_iterator cend() const noexcept { return const_iterator(nullptr); }
        iterator befor_begin() noexcept { return iterator(sentinel); }
        const_iterator befor_begin() const noexcept { return const_iterator(sentinel); }
        const_iterator cbefor_begin() const noexcept { return const_iterator(sentinel); }

        void push_front(const T& value) { insert_after(befor_begin(), value); }
        void pop_front() { erase_after(befor_begin()); }

        void remove(const T& value);

        void reverse() noexcept;

        void merge(my_forward_list<T>& other);
        void merge(my_forward_list<T>&& other);

    private:
        Node* get_middle(Node* head);

        void sort();


        //ostream operator
        friend std::ostream& operator<<(std::ostream& os, my_forward_list<T>& l) {
            Node* node = l.sentinel->next;
            while (node) {
                os << node->value << ' ';
                node = node->next;
            }
            return os;
        }
        

};

//ctor
template<typename T>
my_forward_list<T>::my_forward_list(size_type count) : sentinel(new Node()) {
    while (count--) {
        sentinel->next = new Node(0, sentinel->next);
    }
}

template <typename T>
my_forward_list<T>::my_forward_list(size_type count, const value_type& elem) : sentinel(new Node()) {
    Node* tmp = sentinel;
    while (count--) {
        tmp->next = new Node(elem, tmp->next);
    }
}

template <typename T>
my_forward_list<T>::my_forward_list(const my_forward_list<T>& other) : sentinel(new Node()) {
    Node* r = other.sentinel->next;
    Node* node = sentinel;

    while (r) {
        node->next = new Node(r->value);
        node = node->next;
        r = r->next;
    }
}

template <typename T>
my_forward_list<T>::my_forward_list(my_forward_list<T>&& other) : sentinel(new Node()) {
    sentinel->next = other.sentinel->next;
    other.sentinel->next = nullptr;
}

template <typename T>
my_forward_list<T>::my_forward_list(std::initializer_list<T> init) : sentinel(new Node()) {
    Node* node = sentinel;
    for (auto n : init) {
        node->next = new Node(n);
        node = node->next;
    }
}

//operator assign
template<typename T>
my_forward_list<T>& my_forward_list<T>::operator=(const my_forward_list<T>& other) {
    if (this == &other) return *this;

    this->clear();

    Node* src = other.sentinel->next;
    
    while (src != nullptr) {
        
        src = src->next;
    }

    return *this;
}

template <typename T>
my_forward_list<T>& my_forward_list<T>::operator=(my_forward_list<T>&& other) {
    this->clear();

    insert_after(befor_begin(), other.begin(), other.end());
    other.sentinel->next = nullptr;

    return *this;
}

template <typename T>
my_forward_list<T>& my_forward_list<T>::operator=(std::initializer_list<T> ilist) {
    clear();
    insert_after(befor_begin(), ilist);
    return *this;
}



//clear
template <typename T>
void my_forward_list<T>::clear() {
    Node* dummy = sentinel->next;
    while (dummy) {
        Node *nxt = dummy->next;
        delete dummy;
        dummy = nxt;
    }
    sentinel->next = nullptr;
}

//insert
template <typename T>
typename my_forward_list<T>::iterator my_forward_list<T>::insert_after(const_iterator pos, const T& value) {
    Node* curr = pos.current;

    curr->next = new Node(value, curr->next);
    return iterator(curr->next);
}

template<typename T>
typename my_forward_list<T>::iterator my_forward_list<T>::insert_after(const_iterator pos, T&& value) {
    Node* curr = pos.current;

    curr->next = new Node(std::move(value), curr->next);
    return iterator(curr->next);
}

template <typename T>
typename my_forward_list<T>::iterator my_forward_list<T>::insert_after(const_iterator pos, size_type count, const T& value) {
    Node* curr = pos.current;

    while (count--) {
        curr->next = new Node(value, curr->next);
        curr = curr->next;
    }
    return iterator(curr);
}

template <typename T>
template <class InputIt>
typename my_forward_list<T>::iterator my_forward_list<T>::insert_after(const_iterator pos, InputIt first, InputIt last) {
    
    Node* curr = pos.current;
    while (first != last) {
        curr->next = new Node(*first, curr->next);
        curr = curr->next;
        ++first;
    }

    return iterator(curr);
}

template <typename T>
typename my_forward_list<T>::iterator my_forward_list<T>::insert_after(const_iterator pos, std::initializer_list<T> ilist) {
    Node* curr = pos.current;

    for (auto& value : ilist) {
        curr->next = new Node(value, curr->next);
        curr = curr->next;
    }
    return iterator(curr);
}

//erase_after
template <typename T>
typename my_forward_list<T>::iterator my_forward_list<T>::erase_after(const_iterator pos) {
    Node* curr = pos.current;
    if (curr && curr->next) {
        Node* deleteNode = curr->next;
        curr->next = curr->next->next;
        delete deleteNode;
    }
    return iterator(curr->next);
}

template <typename T>
typename my_forward_list<T>::iterator my_forward_list<T>::erase_after(const_iterator first, const_iterator last) {
    Node* curr = first.current;
    while (curr->next != last.current) {
        Node* deleteNode = curr->next;
        curr->next = deleteNode->next;
        delete deleteNode;
    }
    return iterator(curr->next);
}

//remove
template <typename T>
void my_forward_list<T>::remove(const T& value) {
    auto currIt = befor_begin();

    while (currIt.current->next != nullptr) {
        if (currIt.current->next->value == value) {
            erase_after(currIt);
        } else {
            ++currIt;
        }
    }
}

template <typename T>
void my_forward_list<T>::reverse() noexcept {
    Node* curr = sentinel->next;
    Node* prev = nullptr;

    while (curr) {
        Node* n = curr->next;
        curr->next = prev;
        prev = curr;
        curr = n;
    }
    sentinel->next = prev;
}

template <typename T>
void my_forward_list<T>::merge(my_forward_list<T>& other) {
    if (this == &other || other.empty()) return;

    Node* curr = sentinel;
    Node* l = sentinel->next;
    Node* r = other.sentinel->next;

    while (l && r) {
        
        if (l->value <= r->value) {
            curr->next = l;
            l = l->next;
        } else {
            curr->next = r;
            r = r->next;
        }

        curr = curr->next;
    }

    other.sentinel->next = nullptr;

    curr->next = l ? l : r;
}

template <typename T>
void my_forward_list<T>::merge(my_forward_list<T>&& other) {
    merge(other);
}

template <typename T>
typename my_forward_list<T>::Node* my_forward_list<T>::get_middle(Node* head) {
    typename my_forward_list<T>::Node* fast = head->next;
    typename my_forward_list<T>::Node* slow = head;

    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    return slow;
}


template<typename T>
void my_forward_list<T>::sort() {
    if (!sentinel->next || !sentinel->next->next) return;

    Node* mid = get_middle(sentinel->next);
    
    my_forward_list<T> sencond_half;
    sencond_half.sentinel->next = mid->next;

    mid->next = nullptr;

    this->sort();
    sencond_half.sort();

    this->merge(sencond_half);
}
