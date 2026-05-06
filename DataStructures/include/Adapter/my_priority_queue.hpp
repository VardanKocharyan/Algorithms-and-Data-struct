#pragma once
#include <vector>
#include <type_traits>

template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class my_priority_queue {
    public:
        using container_type  = Container;
        using value_compare   = Compare;
        using value_type	  = typename Container::value_type;
        using size_type	      = typename Container::size_type;
        using reference	      = typename Container::reference;
        using const_reference = typename Container::const_reference;

    private:
        Compare comp;
        Container c;

    public:
        //ctors
        my_priority_queue() : my_priority_queue(Compare(), Container()) {}
        explicit my_priority_queue( const Compare& compare ) : my_priority_queue(compare, Container()) {}
        my_priority_queue( const Compare& compare, const Container& cont ) : comp(compare), c(cont) {}
        my_priority_queue( const Compare& compare, Container&& cont ) : comp(compare), c(std::move(cont)) {}
        my_priority_queue( const my_priority_queue& other ) : comp(other.comp), c(other.c) {}
        my_priority_queue( my_priority_queue&& other ) : comp(other.comp), c(std::move(other.c)) {}
        
        template< class InputIt >
        my_priority_queue( InputIt first, InputIt last, const Compare& compare = Compare() ) : comp(compare), c(first, last) 
        {
            build_heap();
        }
        
        template< class InputIt >
        my_priority_queue( InputIt first, InputIt last, const Compare& compare, const Container& cont ) 
            : comp(compare), c(cont) {
            c.insert(c.end(), first, last);
            build_heap();
        }
        
        template< class InputIt >
        my_priority_queue( InputIt first, InputIt last, const Compare& compare, Container&& cont ) 
            : comp(compare), c(std::move(cont)) {
            c.insert(c.end(), first, last);
            build_heap();
        }

        //dtor
        ~my_priority_queue() = default;

        //operator=
        my_priority_queue& operator=(const my_priority_queue& other);
        my_priority_queue& operator=(my_priority_queue&& other);

        const_reference top() const { return c.front(); }
        bool empty() const { return c.empty(); }
        size_type size() const { return c.size(); }

    private:
        void heapifyup(int i) {
            while (i > 0) {
                int parent = (i - 1) / 2;
                if (!comp(c[parent], c[i])) break;
                std::swap(c[i], c[parent]);
                i = parent;
            }
        }

        void heapify_down(int i, int n) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int largest = i;

            if (left < n && comp(c[i], c[left])) i = left;
            if (right < n && comp(c[i], c[right])) i = right;
            if (i != largest) {
                std::swap(c[i], c[largest]);
                heapify_down(i, n);
            }
        }

        void build_heap() {
            int n = c.size();
            for (int i = (n / 2) - 1; i >= 0; --i) heapify_down(i, n);
        }

    public:
        void push( const value_type& value );
        void push( value_type&& value);
        void pop();

        void swap( my_priority_queue& other ) noexcept { std::swap(c, other.c); }
        
        template< class... Args >
        void emplace( Args&&... args ) { push(value_type(std::forward<Args>(args) ...)); }


};

template <typename T, typename Compare, typename Container>
my_priority_queue<T, Compare, Container>& 
    my_priority_queue<T, Compare, Container>::operator=(const my_priority_queue<T, Compare, Container>& other) {
    comp = other.comp;
    c = other.c;
    return *this;
}

template <typename T, typename Compare, typename Container>
my_priority_queue<T, Compare, Container>& 
    my_priority_queue<T, Compare, Container>::operator=(my_priority_queue<T, Compare, Container>&& other) {
    comp = std::move(other.comp);
    c = std::move(other.c);
    return *this;
}

template <typename T, typename Compare, typename Container>
void my_priority_queue<T, Compare, Container>::push( const value_type& value ) {
    c.push_back(value);
    heapifyup(c.size() - 1);
}

template <typename T, typename Compare, typename Container>
void my_priority_queue<T, Compare, Container>::push( value_type&& value ) {
    c.push_back(std::move(value));
    heapifyup(c.size() - 1);
}

template <typename T, typename Compare, typename Container>
void my_priority_queue<T, Compare, Container>::pop() {
    std::swap(c[0], c[c.size() - 1]);
    c.pop_back();
    heapify_down(0, c.size());
}

