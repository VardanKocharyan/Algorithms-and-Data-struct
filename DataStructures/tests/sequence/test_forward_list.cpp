#include <mystl/sequence/forward_list.hpp>
#include <cassert>
#include <iostream>
#include <vector>
#include <string>

void test_push_pop() {
    mystl::forward_list<int> fl;
    fl.push_front(10);
    fl.push_front(20);
    fl.push_front(30);

    assert(!fl.empty());
    assert(fl.front() == 30);
    
    fl.pop_front();
    assert(fl.front() == 20);
    
    fl.pop_front();
    assert(fl.front() == 10);
    
    fl.pop_front();
    assert(fl.empty());
    std::cout << "[PASS] Test Push/Pop\n";
}

void test_insert_erase() {
    mystl::forward_list<int> fl;
    auto it = fl.before_begin();
    it = fl.insert_after(it, 1);
    it = fl.insert_after(it, 2);
    fl.insert_after(it, 3); // List: 1 -> 2 -> 3

    it = fl.begin();
    assert(*it == 1);
    
    fl.erase_after(it); // Erases 2
    
    std::vector<int> expected = {1, 3};
    int idx = 0;
    for (int val : fl) {
        assert(val == expected[idx++]);
    }
    std::cout << "[PASS] Test Insert/Erase\n";
}

void test_reverse_sort_merge() {
    mystl::forward_list<int> l1;
    l1.push_front(1);
    l1.push_front(3);
    l1.push_front(5); // 5 -> 3 -> 1

    l1.reverse(); // 1 -> 3 -> 5
    assert(l1.front() == 1);

    mystl::forward_list<int> l2;
    l2.push_front(2);
    l2.push_front(4);
    l2.push_front(6); // 6 -> 4 -> 2
    l2.sort();        // 2 -> 4 -> 6

    l1.merge(l2);     // 1 -> 2 -> 3 -> 4 -> 5 -> 6
    assert(l2.empty());

    std::vector<int> expected = {1, 2, 3, 4, 5, 6};
    int idx = 0;
    for (int val : l1) {
        assert(val == expected[idx++]);
    }
    std::cout << "[PASS] Test Reverse/Sort/Merge\n";
}

void test_copy_move() {
    mystl::forward_list<std::string> original;
    original.push_front("C++");
    original.push_front("Modern");

    // Copy constructor test
    mystl::forward_list<std::string> copy = original;
    assert(copy.front() == "Modern");

    // Move constructor test
    mystl::forward_list<std::string> moved = std::move(copy);
    assert(moved.front() == "Modern");
    assert(copy.empty());

    std::cout << "[PASS] Test Copy/Move Semantics\n";
}

int main() {
    test_push_pop();
    test_insert_erase();
    test_reverse_sort_merge();
    test_copy_move();
    
    std::cout << "\n>>> ALL FORWARD_LIST TESTS PASSED! <<<\n";
    return 0;
}
