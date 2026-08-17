#include <iostream>
#include <cassert>
#include <string>
#include "../../include/mystl/adaptors/queue.hpp"

void test_queue_fifo() {
    mystl::queue<int> q;
    assert(q.empty());

    q.push(10);
    q.push(20);
    q.push(30);

    assert(q.size() == 3);
    assert(q.front() == 10); 
    assert(q.back() == 30);  

    q.pop(); 
    assert(q.front() == 20);
    assert(q.size() == 2);

    q.pop(); 
    assert(q.front() == 30);

    q.pop();
    assert(q.empty());

    std::cout << "[SUCCESS] Queue FIFO logic test passed!\n";
}

int main() {
    std::cout << "--- Running mystl::queue Tests ---\n";
    test_queue_fifo();
    std::cout << "--- All mystl::queue Tests Passed! ---\n";
    return 0;
}
