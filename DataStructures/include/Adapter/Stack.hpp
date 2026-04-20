#pragma once 
#include <vector>
#include <exception>

template <typename T>
class my_stack {

    public:
        using size_type = typename std::vector<T>::size_type;
        using value_type = typename std::vector<T>::value_type;

    protected:
        std::vector<T> c;

    public:
        class ReadEmptyStack : public std::exception {
            virtual const char* what() const noexcept override {
                return "Read Empty Stack!!!";
            }
        };

        size_type size() { return c.size(); }
        bool empty() { return c.empty(); }

        void push(const T& elem) { c.push_back(elem); }

        T pop() {
            if (empty()) {
                throw ReadEmptyStack();    
            }

            T elem(c[c.size() - 1]);
            c.pop_back();
            return elem;
        }

        T& top() {
            if (empty()) {
                throw ReadEmptyStack();
            }    
            return c.back();    
        }
};

