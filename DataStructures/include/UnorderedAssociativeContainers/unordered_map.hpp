#pragma once 
#include "lib_unordered_map.hpp"

template <
        typename Key,
        typename T,
        typename Hash = std::hash<Key>,
        typename KeyEqual = equal_to<Key> 
> class my_unordered_map 
{
    public:
        using key_type             = Key;
        using mapped_type          = T;
        using value_type           = std::pair<const Key, T>;
        using size_type            = size_t;
        using difference_type      = std::ptrdiff_t;
        using hasher               = Hash;
        using key_equal            = KeyEqual;
        using reference            = value_type&;
        using const_reference      = const value_type&;
        using iterator             = MapIterator<value_type, my_unordered_map>;
        using const_iterator       = MapIterator<const value_type, const my_unordered_map>;
        using local_iterator       = typename my_forward_list<value_type>::iterator;
        using const_local_iterator = typename my_forward_list<value_type>::const_iterator;

    private:
        hasher hash_fn;
        key_equal eq_fn;
        std::vector<my_forward_list<value_type>> table;
        int m;
        int n;
        float max_load_factor = 0.75;

        size_type get_bucket_index(const Key& key) const { return hash_fn(key) % m; }

    public:
        //iterator
        template <typename DataType, typename MapType>
        struct MapIterator 
        {
            using iterator_category = std::forward_iterator_tag;
            using value_type = DataType;
            using reference = DataType&;
            using pointer = DataType*;

            typename my_forward_list<DataType>::Node* current_node;
            size_type bucket_idx;
            MapType* map_ptr;

            MapIterator(typename my_forward_list<DataType>::Node* node, size_type idx, MapType* map)
                : current_node(node), bucket_idx(idx), map_ptr(map) {}

            reference operator*() const { return current_node->value; }
            pointer operator->() const { return &(current_node->value); }

            MapIterator& operator++() {
                if (current_node)  {
                    current_node = current_node->next;
                }

                if (!current_node) {
                    while (!current_node && bucket_idx < m) {
                        if (!table[bucket_idx].empty()) {
                            current_node = table[bucket_idx].begin().current;
                            return *this;
                        }
                        ++bucket_idx;
                    }
                    current_node = nullptr;
                }
                return *this;
            }

            MapIterator operator++(int) {
                MapIterator tmp = *this;
                ++(*this);
                return tmp;
            }
        };
        
    public:
        //ctors
        my_unordered_map() = default;
        
        explicit my_unordered_map(size_type bucket_count,
                                  const Hash& hash = Hash(),
                                  const key_equal& equal = key_equal() );
        
        /*template <typename InputIt>
        my_unordered_map(InputIt first, InputIt last,
                         size_type bucket_count = 13,
                         const Hash& hash = Hash(),
                         const key_equal& equal = key_equal() );
        */

        my_unordered_map(const my_unordered_map& other);
        my_unordered_map(my_unordered_map&& other);
        my_unordered_map(std::initialize_list<value_type> init);
        
        //dtor
        ~my_unordered_map() = default;

        size_type bucket_count() const { return m; }
        float max_load_factor() const { return max_load_factor; }
        size_type bucket_size( size_type n ) const { return n; }
        float load_factor() const { return (bucket_size() / bucket_count()); }

        void rehash(size_type count);

        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        const_iterator cbegin() const noexcept;

        iterator end() noexcept;
        const_iterator end() const noexcept;
        const_iterator cend() const noexcept;
        
        iterator find(const Key& key);
        const_iterator find(const Key& key) const;

        bool contains(const Key& key) const;

        std::pair<iterator, bool> insert( const value_type& value );
        std::pair<iterator, bool> insert( value_type&& value );
        template <typename P>
        std::pair<iterator, bool> insert(P&& value);
        template <typename InputIt>
        void insert(InputIt first, InputIt last);
        void insert(std::initializer_list<value_type> ilist);

        T& operator[](const Key& key);
        T& operator[](Key&& key);
        template <typename K>
        T& operator[](K&& key);

        template <class M>
        std::pair<iterator, bool> insert_or_assign( const Key& key, M&& obj );
        template <class M>
        std::pair<iterator, bool> insert_or_assign( Key&& key, M&& obj );

        iterator erase( iterator pos );
        iterator erase( const_iterator pos );
        iterator erase( const_iterator first, const_iterator last );
        size_type erase( const Key& key );


        friend std::ostream& operator<<(std::ostream& os, const my_unordered_map& l) {
            os << "{ ";    
            for (int i{}; i < l.m; ++i) {
                auto it = l.table[i].begin(); 
                auto end = l.table[i].end();
        
                while (it != end) {
                os << it->first << ": " << it->second << " ";
                ++it;
                }
            }
            os << "}";
            return os;
        }
        
};

template <typename Key, typename T, typename Hash, typename KeyEqual>
my_unordered_map<Key, T, Hash, KeyEqual>::my_unordered_map(
        size_t bucket_count, const Hash& hash, const KeyEqual& equal
) : hash_fn(hash), eq_fn(equal), table(bucket_count), m(bucket_count), n(0) {}


template <typename Key, typename T, typename Hash, typename KeyEqual>
my_unordered_map<Key, T, Hash, KeyEqual>::my_unordered_map(
        const my_unordered_map<Key, T, Hash, KeyEqual>& other
) : hash_fn(other.hash_fn), eq_fn(other.eq_fn), table(other.table), m(other.m), n(other.n) {}

template <typename Key, typename T, typename Hash, typename KeyEqual>
my_unordered_map<Key, T, Hash, KeyEqual>::my_unordered_map(
        my_unordered_map<Key, T, Hash, KeyEqual>&& other
) : hash_fn(other.hash_fn),
    eq_fn(other.eq_fn),
    table(std::move(other.table)),
    m(std::move(other.m)),
    n(std::move(other.n)) 
{
    other.m = 0;
    other.n = 0;
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
my_unordered_map<Key, T, Hash, KeyEqual>::my_unordered_map(
        std::initializer_list<value_type> init
) : hash_fn(Hash()), eq_fn(other.eq_fn), m(13), n(0) 
{
    for (auto& num : init) {
        insert(num);
    }
}

//rehash
template <typename Key, typename T, typename Hash, typename KeyEqual>
void my_unordered_map<Key, T, Hash, KeyEqual>::rehash(size_t count) 
{
    std::vector<my_forward_list<value_type>> new_table(count);

    for (auto& old : table) {
        while (!old.empty()) {
                value_type tmp = std::move(old.front());
                old.pop_front();

                size_type new_idx = hash_fn(tmp.first) % count;
                new_table[new_idx].push_front(std::move(tmp));
        }
    }
    table = std::move(new_table);
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
typename my_unordered_map<Key, T, Hash, KeyEqual>::iterator 
    my_unordered_map<Key, T, Hash, KeyEqual>::begin() 
{
        for (size_type i{}; i < m; ++i) {
            if (!table[i].empty()) {
                return iterator(table[i].begin().current, i, this);
            }
        }
        return end();
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
typename my_unordered_map<Key, T, Hash, KeyEqual>::const_iterator 
    my_unordered_map<Key, T, Hash, KeyEqual>::begin() const 
{
    for (size_type i{}; i < m; ++i) {
        if (!table[i].empty()) {
            return const_iterator(table[i].begin().current, i, this);
        }
    }
    return end();
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
typename my_unordered_map<Key, T, Hash, KeyEqual>::const_iterator 
    my_unordered_map<Key, T, Hash, KeyEqual>::cbegin() const 
{
    for (size_type i{}; i < m; ++i) {
        if (!table[i].empty()) {
            return const_iterator(table[i].cbegin().current, i, this);
        }
    }
    return cend();
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
typename my_unordered_map<Key, T, Hash, KeyEqual>::iterator 
    my_unordered_map<Key, T, Hash, KeyEqual>::end() 
{
    return iterator(nullptr, m, this);
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
typename my_unordered_map<Key, T, Hash, KeyEqual>::const_iterator 
    my_unordered_map<Key, T, Hash, KeyEqual>::end() const
{
    return const_iterator(nullptr, m, this);
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
typename my_unordered_map<Key, T, Hash, KeyEqual>::const_iterator 
    my_unordered_map<Key, T, Hash, KeyEqual>::cend() const
{
    return const_iterator(nullptr, m, this);
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
typename my_unordered_map<Key, T, Hash, KeyEqual>::iterator
    my_unordered_map<Key, T, Hash, KeyEqual>::find(const Key& key) 
{
    size_type idx = get_bucket_index(key);
    auto it = table[idx].begin();
    auto end = table[idx].end();

    while (it != end) 
    {
        if (eq_fn(it->first, key)) return iterator(it.current, idx, this);
        ++it;
    }
    return end();
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
typename my_unordered_map<Key, T, Hash, KeyEqual>::const_iterator
    my_unordered_map<Key, T, Hash, KeyEqual>::find(const Key& key) const
{
    size_type idx = get_bucket_index(key);
    auto it = table[idx].begin();
    auto end = table[idx].end();

    while (it != end) 
    {
        if (eq_fn(it->first, key)) return const_iterator(it.current, idx, this);
        ++it;
    }
    return end();
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
bool my_unordered_map<Key, T, Hash, KeyEqual>::contains(const Key& key) const
{
    return find(key) != end();
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
std::pair<typename my_unordered_map<Key, T, Hash, KeyEqual>::iterator, bool> 
    my_unordered_map<Key, T, Hash, KeyEqual>::insert( const value_type& value )
{
    if (load_factor() > max_load_factor()) rehash(table.size() * 2);

    iterator it = find(value.first);
    if (it != end()) return {it, false};

    size_type idx = get_bucket_index(value.first);
    table[idx].push_front(value);
    ++n;

    return {iterator(table[idx].begin().current, idx, this), true};
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
std::pair<typename my_unordered_map<Key, T, Hash, KeyEqual>::iterator, bool> 
    my_unordered_map<Key, T, Hash, KeyEqual>::insert(value_type&& value) 
{
    if (load_factor() > max_load_factor()) rehash(table.size() * 2);  

    iterator it = find(value.first);
    if (it != end()) return {it, false};

    size_type idx = get_bucket_index(value.first);
    table[idx].push_front(std::move(value));
    ++n;

    return {iterator(table[idx].begin().current, idx, this), true};
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
template<typename P>
std::pair<typename my_unordered_map<Key, T, Hash, KeyEqual>::iterator, bool>
    my_unordered_map<Key, T, Hash, KeyEqual>::insert(P&& value)
{
    if (load_factor() > max_load_factor()) rehash(table.size() * 2);

    value_type tmp(std::forward<P>(value));

    auto it = find(tmp.first);
    if (it != end()) {
        return {it, false};
    }

    size_t idx = get_bucket_index(tmp.first);
    table[idx].push_front(std::move(tmp));
    ++n;

        return {iterator(table[idx].begin().current, idx, this), true};
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
template <typename InputIt>
void my_unordered_map<Key, T, Hash, KeyEqual>::insert(InputIt first, InputIt last) 
{
    while (first != last) 
    {
        insert(*first);
        ++first;
    }
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
void my_unordered_map<Key, T, Hash, KeyEqual>::insert(std::initializer_list<value_type> ilist)
{
    for (const auto& p : ilist) {
        insert(p);
    }
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
T& my_unordered_map<Key, T, Hash, KeyEqual>::operator[](const Key& key)
{
    if (load_factor() > max_load_factor()) rehash(table.size() * 2);

    auto it = find(key);
    if (it != end()) return it->second;
    return insert({key, T()}).first->second;
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
T& my_unordered_map<Key, T, Hash, KeyEqual>::operator[](Key&& key)
{
    if (load_factor() > max_load_factor()) rehash(table.size() * 2);

    auto it = find(std::move(key));
    if (it != end()) return it->second;
    return insert({std::move(key), T()}).first->second;
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
template <typename K>
T& my_unordered_map<Key, T, Hash, KeyEqual>::operator[](K&& key)
{
    if (load_factor() > max_load_factor()) rehash(table.size() * 2);

    auto it = find(key);
    if (it != end()) return it->second;
    return insert({std::forward(key), T()}).first->second;
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
template <class M>
std::pair<iterator, bool> my_unordered_map<Key, T, Hash, KeyEqual>::insert_or_assign( const Key& key, M&& obj )
{
    auto it = find(key);
    if (it != end())
    {
        it->second = std::forward<M>(obj);
        return {it, false};
    }
    return insert({key, std::forward<M>(obj)});
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
template <class M>
std::pair<iterator, bool> 
    my_unordered_map<Key, T, Hash, KeyEqual>::insert_or_assign( Key&& key, M&& obj )
{
    auto it = find(std::move(key));
    if (it != end()) {
        it->second = std::forward<M>(obj);
        return {it, false};
    }
    return insert({std::move(key), std::forward<M>(obj)});
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
typename my_unordered_map<Key, T, Hash, KeyEqual>::iterator
    my_unordered_map<Key, T, Hash, KeyEqual>::erase( iterator pos )
{
    if (pos == end()) return end();

    size_type idx = pos.idx;
    auto& bucket = table[idx];

    auto prev = bucket.befor_begin();
    auto curr = bucket.begin();

    if (curr != bucket.end() && curr.current != pos.current)
    {
        prev = curr;
        ++curr;
    }

    auto next_local = bucket.erase_after(prev);
    --n;

    while (next_local != bucket.end())
    {
        return iterator(next_local.current, idx, this);
    } else {
        iterator res(nullptr, idx, this);
        return ++res;
    }
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
typename my_unordered_map<Key, T, Hash, KeyEqual>::iterator
    my_unordered_map<Key, T, Hash, KeyEqual>::erase( const_iterator pos )
{
    if (pos == end()) return end();

    size_type idx = pos.idx;
    auto& bucket = table[idx];

    auto prev = bucket.befor_begin();
    auto curr = bucket.begin();

    while (curr != bucket.end() && curr.current != pos.current)
    {
        prev = curr;
        ++curr;
    }

    auto next_local = bucket.erase_after(prev);
    --n;

    if (next_local != bucket.end())
    {
        return iterator(next_local.current, idx, this);
    } else {
        iterator res(nullptr, idx, this);
        return ++res;
    }
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
size_t my_unordered_map<Key, T, Hash, KeyEqual>::erase( const Key& key )
{
    size_t idx = get_bucket_index(key);
    auto& bucket = table[idx];

    auto prev = bucket.befor_begin();
    auto curr = bucket.begin();

    while (curr != bucket.end()) {
        if ( eq_fn(curr->first, key) ) 
        {
            bucket.erase_after(prev);
            --n;
            return 1;
        } else
        {
            prev = curr;
            ++curr;
        }
    }
        return 0;
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
typename my_unordered_map<Key, T, Hash, KeyEqual>::iterator 
    erase( const_iterator first, const_iterator last )
{
        while (first != last) first = erase(first);
        return iterator(first.current, first.idx, this); //last
}
