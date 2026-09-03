#pragma once 

#include <cstddef>
#include <type_traits>
#include <cmath>

#include "../sequence/vector.hpp"


namespace mystl {

template <
    class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>
> class unordered_map {

public:
    using key_type               = Key;
    using mapped_type           = T;
    using value_type            = std::pair<const Key, T>;
    using size_type             = std::size_t;
    using difference_type       = std::ptrdiff_t;
    using hasher                = Hash;
    using key_equal             = KeyEqual;
    using allocator_type        = Allocator;
    using reference             = value_type&;
    using const_reference       = const value_type&;
    using pointer               = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer         = typename std::allocator_traits<Allocator>::const_pointer;
    
private:
    struct Node {
        value_type value;
        Node* next{nullptr};
        std::size_t hash_code;
    };

public:
    using node_allocator        = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    using node_traits           = std::allocator_traits<node_allocator>;

private:
    template <typename... Args>
    Node* create_node(Args&&... args) {
        Node* p = node_traits::allocate(node_alloc_, 1);

        try {
            node_traits::construct(node_alloc_, std::addressof(p->value), std::forward<Args>(args)...);        
        } catch(...) {
            node_traits::deallocate(node_alloc_, p, 1);
            throw;
        }
        p->next = nullptr;
        p->hash_code = 0;
        return p;
    }

    void destroy_node(Node* p) {
        node_traits::destroy(node_alloc_, std::addressof(p->value));
        node_traits::deallocate(node_alloc_, p, 1);
    }

public:
    template <bool IsConst>
    class unordered_map_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = std::pair<const Key, T>;
        using difference_type   = std::ptrdiff_t;
        using pointer           = std::conditional_t<IsConst, const value_type*, value_type*>;
        using reference         = std::conditional_t<IsConst, const value_type&, value_type&>;
        
        using unordered_map_node_ptr    = std::conditional_t<IsConst, const Node*, Node*>;
        using map_ptr                   = std::conditional_t<IsConst, const unordered_map*, unordered_map*>;

        unordered_map_iterator() = default;
        unordered_map_iterator(Node* node, map_ptr map, size_type bucket_idx)
            : current_node_(node), map_(map), bucket_index_(bucket_idx) {}

        template <bool OtherConst, typename = std::enable_if_t<!OtherConst && IsConst>>
        unordered_map_iterator(const unordered_map_iterator<OtherConst>& other) noexcept 
            : current_node_(other.current_node_), map_(other.map_), bucket_index_(other.bucket_index_) {}
            
        reference operator*() const noexcept { return current_node_->value; };

        pointer operator->() const noexcept { return std::addressof(operator*()); }

        unordered_map_iterator& operator++() noexcept {
            if (current_node_->next) {
                current_node_ = current_node_->next;
            } else {
                current_node_ = nullptr;
                ++bucket_index_;
                while (bucket_index_ < map_->bucket_.size()) {
                    if (map_->bucket_[bucket_index_]) {
                        current_node_ = map_->bucket_[bucket_index_];
                        break;
                    }
                    ++bucket_index_;
                }
            }

            return *this;
        }

        unordered_map_iterator& operator++(int) noexcept {
            unordered_map_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
    
        friend bool operator==(const unordered_map_iterator& lhs, const unordered_map_iterator& rhs) noexcept {
            return lhs.current_node_ == rhs.current_node_;
        }

        friend bool operator!=(const unordered_map_iterator& lhs, const unordered_map_iterator& rhs) noexcept {
            return !(lhs == rhs);
        }

    private:
        unordered_map_node_ptr current_node_{nullptr};
        map_ptr map_{nullptr};
        size_type bucket_index_{0};

        friend class unordered_map;
        template <bool> friend class unordered_map_iterator;
    };

    using iterator          = unordered_map_iterator<false>;
    using const_iterator    = unordered_map_iterator<true>;


    iterator begin() noexcept {
        for (size_type i = 0; i < bucket_count_; ++i) {
            if (bucket_[i] != nullptr) {
                return iterator(bucket_[i], this, i);
            }
        }
        return end();
    }

    iterator end() noexcept {
        return iterator(nullptr, this, bucket_count_);
    }

    const_iterator cbegin() const noexcept {
        for (size_type i = 0; i < bucket_count_; ++i) {
            if (bucket_[i]) return const_iterator(bucket_[i], this, i);
        }
        return cend();
    }

    const_iterator cend() const noexcept {
        return const_iterator(nullptr, this, bucket_count_);
    }


    explicit unordered_map(size_type bucket_count = 16,
                       const Hash& hash = Hash(),
                       const KeyEqual& equal = KeyEqual(),
                       const Allocator& alloc = Allocator())
    : hash_(hash),
      key_equal_(equal),
      bucket_(bucket_count),
      bucket_count_(bucket_count),
      node_alloc_(alloc) {}

    unordered_map(std::initializer_list<value_type> init,
                size_type bucket_count = 8,
                const hasher& hash = hasher(),
                const key_equal& equal = key_equal(),
                const allocator_type& alloc = allocator_type())
        : unordered_map(bucket_count, hash, equal, alloc) {
        for (const auto& item : init) {
            emplace(item);
        }
    }

    template <typename InputIt>
    unordered_map(InputIt first, InputIt last,
                size_type bucket_count = 8,
                const hasher& hash = hasher(),
                const key_equal& equal = key_equal(),
                const allocator_type& alloc = allocator_type())
        : unordered_map(bucket_count, hash, equal, alloc) {
        for (; first != last; ++first) {
            emplace(*first);
        }
    }

    unordered_map(const unordered_map& other)
        : bucket_count_(other.bucket_count_),
            hash_(other.hash_),
            key_equal_(other.key_equal_),
            node_alloc_(other.node_alloc_),
            size_(0),
            max_load_factor_(other.max_load_factor_),
            bucket_(other.bucket_count_, nullptr) {
        for (const auto& item : other) {
            emplace(item);
        }
    }

    unordered_map& operator=(const unordered_map& other) {
        if (this != &other) {
            clear();
            hash_ = other.hash_;
            key_equal_ = other.key_equal_;
            node_alloc_ = other.node_alloc_;
            max_load_factor_ = other.max_load_factor_;
            rehash(other.bucket_count_);
        
            for (const auto& item : other) {
                emplace(item);
            }
        }
        return *this;
    }

    unordered_map(unordered_map&& other) noexcept
        : bucket_count_(std::exchange(other.bucket_count_, 0)),
            hash_(std::move(other.hash_)),
            key_equal_(std::move(other.key_equal_)),
            node_alloc_(std::move(other.node_alloc_)),
            size_(std::exchange(other.size_, 0)),
            max_load_factor_(other.max_load_factor_),
            bucket_(std::move(other.bucket_)) {}

    unordered_map& operator=(unordered_map&& other) noexcept {
        if (this != &other) {
            clear();
            bucket_count_ = std::exchange(other.bucket_count_, 0);
            hash_ = std::move(other.hash_);
            key_equal_ = std::move(other.key_equal_);
            node_alloc_ = std::move(other.node_alloc_);
            size_ = std::exchange(other.size_, 0);
            max_load_factor_ = other.max_load_factor_;
            bucket_ = std::move(other.bucket_);
        }
        return *this;
    }



    ~unordered_map() {
        clear();
    }


    
    iterator find( const Key& key ) {
        if (bucket_[hash_(key) % bucket_count_]){
            Node* ptr = bucket_[hash_(key) % bucket_count_];
            while (ptr && ptr->value.first != key) {
                ptr = ptr->next;
            }
            if (ptr) return iterator(ptr, this, hash_(key) % bucket_count_);
        }         
        return end();
    }

    const_iterator find( const Key& key ) const {
        if (bucket_[hash_(key) % bucket_count_]){
            Node* ptr = bucket_[hash_(key) % bucket_count_];
            while (ptr && ptr->value.first != key) {
                ptr = ptr->next;
            }
            if (ptr) return const_iterator(ptr, this, hash_(key) % bucket_count_);
        }         
        return cend();
    }

    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        Node* ptr = create_node(0, std::forward<Args>(args)...);

        const Key& key = ptr->value.first;
        std::size_t hash_code = hash_(key);
        ptr->hash_code = hash_code;

        if (bucket_count_ > 0) {
            size_type idx = hash_code % bucket_count_;
            Node* curr = bucket_[idx];
            while (curr != nullptr) {
                if (key_equal_(curr->value.first, key)) {
                    destroy_node(ptr);
                    return {iterator(curr, this, idx), false};
                }
                curr = curr->next;
            }
        }

        if (bucket_count_ == 0 || size_ + 1 > bucket_count_ * max_load_factor_) {
            rehash(std::max(size_type(8), bucket_count_ * 2));
        }

        size_type idx = hash_code % bucket_count_;

        ptr->next = bucket_[idx];
        bucket_[idx] = ptr;
        ++size_;

        return {iterator(ptr, this, idx), true};
    }

    std::pair<iterator, bool> insert( const value_type& value ) {
        return emplace(value);
    }

    std::pair<iterator, bool> insert( value_type&& value ) {
        return emplace(std::move(value));
    }

    T& operator[](const Key& key) {
        return insert(value_type(key, T())).first->second;
    }

    T& operator[](Key&& key) {
        return insert(value_type(std::move(key), T())).first->second;
    }

    T& at(const Key& key) {
        iterator it = find(key);
        if (it == end()) {
            throw std::out_of_range("mystl::unordered_map::at: key not found");
        }
        return it->second;
    }

    const T& at(const Key& key) const {
        const_iterator it = find(key);
        if (it == end()) {
            throw std::out_of_range("mystl::unordered_map::at: key not found");
        }
        return it->second;
    }

    size_type erase(const key_type& key) {
        if (bucket_count_ == 0) return 0;
    
        std::size_t hash_code = hash_(key);
        size_type idx = hash_code % bucket_count_;

        Node* curr = bucket_[idx];
        Node* prev = nullptr;

        while (curr) {
            if (key_equal_(curr->value.first, key)) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    bucket_[idx] = curr->next;
                }
                destroy_node(curr);
                --size_;
                return 1;
            }
            prev = curr;
            curr = curr->next;
        }
        return 0;
    }

    iterator erase(const_iterator pos) {
        if (pos == end()) return end();

        const_iterator next_it = pos;
        ++next_it;

        Node* node_to_delete = const_cast<Node*>(pos.current_node_);
        size_type idx = node_to_delete->hash_code % bucket_count_;

        Node* curr = bucket_[idx];
        Node* prev = nullptr;

        while (curr) {
            if (curr == node_to_delete) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    bucket_[idx] = curr->next;
                }
                destroy_node(curr);
                --size_;
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        return iterator(const_cast<Node*>(next_it.current_node_), this, next_it.bucket_index_);
    }

    iterator erase(const_iterator first, const_iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return iterator(const_cast<Node*>(last.current_node_), this, last.bucket_index_);
    }

    float load_factor() const noexcept {
        if (bucket_count_ == 0) return 0.0f;
        return static_cast<float>(size_) / static_cast<float>(bucket_count_);
    }

    void max_load_factor(float mlf) {
        max_load_factor_ = mlf;
    }

    void reserve(size_type count) {
        rehash(static_cast<size_type>(std::ceil(static_cast<float>(count) / max_load_factor_)));
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    
    [[nodiscard]] float max_load_factor() const noexcept {
        return max_load_factor_;
    }

    size_type bucket_count() const noexcept {
        return bucket_count_;
    }


    void rehash(size_type count) {
        size_type target_bucket_count = std::max(count, static_cast<size_type>(std::ceil(static_cast<float>(size_) / max_load_factor_)));
    
        if (target_bucket_count <= bucket_count_) {
            return;
        }

        vector<Node*> new_buckets(target_bucket_count, nullptr);

        for (size_type i = 0; i < bucket_count_; ++i) {
            Node* curr = bucket_[i];
            while (curr != nullptr) {
                Node* next_node = curr->next;

                size_type new_idx = curr->hash_code % target_bucket_count;

                curr->next = new_buckets[new_idx];
                new_buckets[new_idx] = curr;

                curr = next_node;
            }
        }

        bucket_ = std::move(new_buckets);
        bucket_count_ = target_bucket_count;
    }

    void clear() noexcept {
        for (size_type i = 0; i < bucket_count_; ++i) {
            Node* curr = bucket_[i];
            while (curr != nullptr) {
                Node* next = curr->next;
                destroy_node(curr);
                curr = next;
            }
            bucket_[i] = nullptr;
        }
        size_ = 0;
    }



private:
    hasher hash_;
    key_equal key_equal_;
    vector<Node*> bucket_;
    size_type size_{};
    size_type bucket_count_{};
    float max_load_factor_{1.0f};
    [[no_unique_address]] node_allocator node_alloc_;






};

} //mystl namespace
