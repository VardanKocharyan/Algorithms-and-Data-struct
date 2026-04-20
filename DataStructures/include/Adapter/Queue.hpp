#pragma once 
#include <list>
#include <exception>

template <typename T>
class my_queue {
    public:
        using value_type = typename std::list<T>::value_type;
        using size_type = typename std::list<T>::size_type;

    protected:
        std::list<T> c;
    
    public:
        class EmptyQueueException : public std::exception {
            const char* what() const noexcept override {
                return "Empty Queue!!!";    
            }
        };

        void push(const T& elem) { c.push_back(elem); }

        void pop() {
            if (empty()) {
                throw EmptyQueueException();
            }
            c.pop_front(); 
        }

        T& front() {
            if (empty()) {
                throw EmptyQueueException();
            }
            return c.front(); 
        }

        T& back() { 
            if (empty()) {
                throw EmptyQueueException();
            }

            return c.back(); }
        size_type size() { return c.size(); }
        bool empty() { return c.empty(); }
};
