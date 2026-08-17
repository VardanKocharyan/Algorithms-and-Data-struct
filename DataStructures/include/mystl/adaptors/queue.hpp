#pragma once 

#include <utility>
#include <cstddef>
#include "../sequence/deque.hpp"

namespace mystl {

template <
    class T,
    class Container = deque<T>
>class queue {
public:
    using container_type    = Container;
    using value_type        = typename Container::value_type;
    using size_type         = typename Container::size_type;
    using reference         = typename Container::reference;
    using const_reference   = typename Container::const_reference;

    queue() = default;
    explicit queue( const Container& cont ) : c(cont) {}
    explicit queue ( Container&& cont) : c(std::move(cont)) {}
    queue( const queue& other ) = default;
    queue( queue&& other ) = default;
    queue& operator=( const queue& other ) = default;
    queue& operator=( queue&& other ) = default;
    ~queue() = default;

    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }

    reference back() { return c.back(); }
    const_reference back() const { return c.back(); }

    size_type size() const noexcept { return c.size(); }

    [[nodiscard]] bool empty() const noexcept { return c.empty(); }

    void push( const T& value ) { c.push_back(value); }
    void push( T&& value ) { c.push_back(std::move(value)); }

    template < class... Args>
    decltype(auto) emplace( Args&&... args ) {
        return c.emplace_back(std::forward<Args>(args)...);
    }

    void pop() { c.pop_front(); }

    void swap( queue& other ) noexcept(noexcept(std::swap(c, other.c))) {
        using std::swap;
        swap(c, other.c);
    }

    friend void swap( queue& lhs, queue& rhs ) noexcept(noexcept(lhs.swap(rhs))) {
        lhs.swap(rhs);
    }


protected:
    Container c;
};

} //mystl namespace
