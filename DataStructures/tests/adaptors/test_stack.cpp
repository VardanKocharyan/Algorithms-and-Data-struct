#include <iostream>
#include <cassert>
#include <string>
#include <mystl/adaptors/stack.hpp>
#include <mystl/sequence/vector.hpp>

void test_stack_basic() {
    mystl::stack<int> s;
    assert(s.empty());
    assert(s.size() == 0);

    s.push(10);
    s.push(20);
    s.push(30);

    assert(!s.empty());
    assert(s.size() == 3);
    assert(s.top() == 30);

    s.pop();
    assert(s.top() == 20);
    assert(s.size() == 2);

    s.pop();
    assert(s.top() == 10);

    s.pop();
    assert(s.empty());
    assert(s.size() == 0);

    std::cout << "[SUCCESS] Basic Push/Pop/Top tests passed!\n";
}

void test_stack_emplace_and_strings() {
    mystl::stack<std::string> s;

    // test lvalue & rvalue push
    std::string str = "Hello";
    s.push(str);
    s.push("World");

    // test emplace in-place construction
    s.emplace(5, 'c'); // "ccccc"

    assert(s.size() == 3);
    assert(s.top() == "ccccc");
    s.pop();

    assert(s.top() == "World");
    s.pop();

    assert(s.top() == "Hello");
    s.pop();

    assert(s.empty());
    std::cout << "[SUCCESS] Emplace and Complex Type tests passed!\n";
}

void test_stack_const_access() {
    mystl::stack<int> s;
    s.push(42);

    const auto& const_s = s;
    assert(const_s.top() == 42);
    assert(!const_s.empty());
    assert(const_s.size() == 1);

    std::cout << "[SUCCESS] Const Reference Top tests passed!\n";
}

int main() {
    std::cout << "--- Running mystl::stack Tests ---\n";
    test_stack_basic();
    test_stack_emplace_and_strings();
    test_stack_const_access();
    std::cout << "--- All mystl::stack Tests Passed Successfully! ---\n";
    return 0;
}
