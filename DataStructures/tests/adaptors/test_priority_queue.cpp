#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <functional>
#include <mystl/adaptors/priority_queue.hpp> // Adapt path to your header

struct Task {
    int priority;
    std::string name;

    bool operator<(const Task& other) const {
        return priority < other.priority;
    }
};

void test_default_max_heap() {
    mystl::priority_queue<int> pq;
    assert(pq.empty());
    assert(pq.size() == 0);

    pq.push(10);
    pq.push(30);
    pq.push(20);
    pq.push(5);

    assert(!pq.empty());
    assert(pq.size() == 4);
    assert(pq.top() == 30);

    pq.pop();
    assert(pq.top() == 20);

    pq.pop();
    assert(pq.top() == 10);

    pq.pop();
    assert(pq.top() == 5);

    pq.pop();
    assert(pq.empty());
    std::cout << "[PASSED] Default Max-Heap Test\n";
}

void test_min_heap() {
    mystl::priority_queue<int, mystl::vector<int>, std::greater<int>> min_pq;

    min_pq.push(10);
    min_pq.push(30);
    min_pq.push(20);
    min_pq.push(5);

    assert(min_pq.top() == 5);
    min_pq.pop();
    assert(min_pq.top() == 10);

    std::cout << "[PASSED] Min-Heap Test\n";
}

void test_range_constructor() {
    mystl::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6};
    mystl::priority_queue<int> pq(vec.begin(), vec.end());

    assert(pq.size() == 8);
    assert(pq.top() == 9);

    pq.pop();
    assert(pq.top() == 6);

    std::cout << "[PASSED] Range Constructor ($O(N)$ Heapify) Test\n";
}

void test_emplace_and_move() {
    mystl::priority_queue<std::string> pq;

    std::string str = "lvalue";
    pq.push(str);                   // Copy push
    pq.push("rvalue_temp");         // Move push
    pq.emplace(5, 'a');             // In-place construction "aaaaa"

    assert(pq.size() == 3);
    assert(pq.top() == "rvalue_temp");

    std::cout << "[PASSED] Emplace & Move Test\n";
}

void test_custom_comparator_and_struct() {
    mystl::priority_queue<Task> task_queue;

    task_queue.push({1, "Low Priority"});
    task_queue.push({10, "Critical Bug"});
    task_queue.push({5, "Medium Feature"});

    assert(task_queue.top().name == "Critical Bug");
    task_queue.pop();
    assert(task_queue.top().name == "Medium Feature");

    std::cout << "[PASSED] Custom Struct Test\n";
}

void test_swap() {
    mystl::priority_queue<int> pq1;
    pq1.push(100);
    pq1.push(200);

    mystl::priority_queue<int> pq2;
    pq2.push(5);

    pq1.swap(pq2);

    assert(pq1.size() == 1 && pq1.top() == 5);
    assert(pq2.size() == 2 && pq2.top() == 200);

    // Non-member ADL swap test
    using std::swap;
    swap(pq1, pq2);

    assert(pq1.top() == 200);
    assert(pq2.top() == 5);

    std::cout << "[PASSED] Swap Test\n";
}

int main() {
    std::cout << "--- Starting mystl::priority_queue Unit Tests ---\n";
    
    test_default_max_heap();
    test_min_heap();
    test_range_constructor();
    test_emplace_and_move();
    test_custom_comparator_and_struct();
    test_swap();

    std::cout << "--- All Tests Passed Successfully! ---\n";
    return 0;
}
