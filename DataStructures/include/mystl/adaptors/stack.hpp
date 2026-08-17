#pragma once 

#include <utility>
#include <cstddef>
#include "../sequence/deque.hpp"

namespace mystl {

template <
    class T,
    class Container = deque<T> 
> class stack {
public:
    using container_type    = Container;
    using value_type        = T;
    using size_type         = std::size_t;
    using reference	        = typename Container::reference;
    using const_reference   = typename Container::const_reference;

    
    stack() = default;
    explicit stack( const Container& cont ) : c(cont) {}
    explicit stack( Container&& cont ) : c(std::move(cont)) {}
    stack(const stack& other) = default;
    stack(stack&& other) = default;
    stack& operator=(const stack& other) = default;
    stack& operator=(stack&& other) = default;
    ~stack() = default;


    reference top() { return c.back(); }

    const_reference top() const { return c.back(); }

    size_type size() const noexcept { return c.size(); }

    [[nodiscard]] bool empty() const noexcept { return c.empty(); }

    void push( const T& value ) { c.push_back(value); }

    void push( T&&  value ) { c.push_back(std::move(value)); }

    template< class... Args >
    decltype(auto) emplace( Args&&... args ) {
        return c.emplace_back(std::forward<Args>(args)...);
    }

    void pop() { c.pop_back(); }

protected:
    Container c;
};

} // mystl namespace
