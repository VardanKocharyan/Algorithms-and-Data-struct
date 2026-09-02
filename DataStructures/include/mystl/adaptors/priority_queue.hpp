#pragma once

#include <utility>
#include <cstddef>
#include <functional>
#include "../sequence/vector.hpp"

namespace mystl {

template <
    class T,
    class Container = mystl::vector<T>,
    class Compare   = std::less<typename Container::value_type>
>
class priority_queue {
public:
    using container_type  = Container;
    using value_compare   = Compare;
    using value_type      = typename Container::value_type;
    using size_type       = typename Container::size_type;
    using reference       = typename Container::reference;
    using const_reference = typename Container::const_reference;

protected:
    Container c;
    Compare   comp;

public:
    // 1. Constructors
    explicit priority_queue(const Compare& compare, const Container& cont)
        : c(cont), comp(compare) {}

    explicit priority_queue(const Compare& compare = Compare(), Container&& cont = Container())
        : c(std::move(cont)), comp(compare) {}

    priority_queue(const priority_queue& other) = default;
    priority_queue(priority_queue&& other)      = default;

    // 2. Range Constructors
    template <class InputIt>
    priority_queue(InputIt first, InputIt last, const Compare& compare, const Container& cont)
        : c(cont), comp(compare) {
        c.insert(c.end(), first, last);
        make_heap(); 
    }

    template <class InputIt>
    priority_queue(InputIt first, InputIt last, const Compare& compare = Compare(), Container&& cont = Container())
        : c(std::move(cont)), comp(compare) {
        for (; first != last; ++first) {
            c.push_back(*first);
        }
        std::make_heap(c.begin(), c.end(), comp);
    }

    ~priority_queue() = default;

    priority_queue& operator=(const priority_queue& other) = default;
    priority_queue& operator=(priority_queue&& other)      = default;

protected:
    void sift_up(size_type index) {
        if (index == 0) return;

        value_type value = std::move(c[index]);
        while (index > 0) {
            size_type parent = (index - 1) / 2;
            
            if (comp(c[parent], value)) {
                c[index] = std::move(c[parent]);
                index = parent;
            } else {
                break;
            }
        }
        c[index] = std::move(value);
    }

    void sift_down(size_type index) {
        size_type size = c.size();
        if (index >= size) return;

        value_type value = std::move(c[index]);
        while (2 * index + 1 < size) {
            size_type left = 2 * index + 1;
            size_type right = 2 * index + 2;
            size_type largest = left;

            if (right < size && comp(c[left], c[right])) {
                largest = right;
            }

            if (comp(value, c[largest])) {
                c[index] = std::move(c[largest]);
                index = largest;
            } else {
                break;
            }
        }
        c[index] = std::move(value);
    }

    void make_heap() {
        if (c.size() < 2) return;
        
        for (size_type i = c.size() / 2; i > 0; --i) {
            sift_down(i - 1);
        }
    }

public:
    // 1. Element Access
    const_reference top() const { 
        return c.front(); 
    }

    // 2. Capacity
    [[nodiscard]] bool empty() const noexcept { 
        return c.empty(); 
    }

    size_type size() const noexcept { 
        return c.size(); 
    }

    // 3. Modifiers
    void push(const value_type& value) {
        c.push_back(value);
        sift_up(c.size() - 1);
    }

    void push(value_type&& value) {
        c.push_back(std::move(value));
        sift_up(c.size() - 1);
    }

    template <class... Args>
    decltype(auto) emplace(Args&&... args) {
        c.emplace_back(std::forward<Args>(args)...);
        sift_up(c.size() - 1);
    }

    void pop() {
        if (empty()) return;
        std::swap(c.front(), c.back());
        c.pop_back();
        sift_down(0); // Root starts at index 0
    }

    // Member swap
    void swap(priority_queue& other) noexcept
    {
        c.swap(other.c);
        std::swap(comp, other.comp);
    }


};

    // Non-member free function swap
    template <class T, class Container, class Compare>
    void swap(priority_queue<T, Container, Compare>& x,
              priority_queue<T, Container, Compare>& y) noexcept(noexcept(x.swap(y))) {
        x.swap(y);
    }

} // namespace mystl
