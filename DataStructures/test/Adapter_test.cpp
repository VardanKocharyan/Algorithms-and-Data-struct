#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <string>
#include <cassert>
#include "Adapter.hpp"

// A simple custom struct for testing emplace and custom comparators
struct Task {
    int priority;
    std::string name;
    Task(int p, std::string n) : priority(p), name(n) {}
};

struct TaskCompare {
    bool operator()(const Task& a, const Task& b) {
        return a.priority < b.priority; // Max-heap based on priority
    }
};

void run_tests() {
    std::cout << "Starting tests...\n";

    // 1. Test Default Constructor and basic push/top/pop
    {
        my_priority_queue<int> pq;
        pq.push(10);
        pq.push(30);
        pq.push(20);
        assert(pq.size() == 3);
        assert(pq.top() == 30); // Max element should be top
        pq.pop();
        assert(pq.top() == 20);
        std::cout << "  - Basic Push/Pop: Passed\n";
    }

    // 2. Test Range-based Constructor (Iterator)
    {
        std::vector<int> vec = {1, 5, 2, 8, 3};
        my_priority_queue<int> pq(vec.begin(), vec.end());
        assert(pq.size() == 5);
        assert(pq.top() == 8);
        std::cout << "  - Iterator Constructor: Passed\n";
    }

    // 3. Test Custom Container (deque) and Min-heap (greater)
    {
        std::vector<int> data = {10, 2, 15, 7};
        // Using std::greater makes it a Min-heap
        my_priority_queue<int, std::deque<int>, std::greater<int>> min_pq(data.begin(), data.end());
        assert(min_pq.top() == 2);
        std::cout << "  - Custom Container/Min-heap: Passed\n";
    }

    // 4. Test Emplace (with custom struct)
    {
        my_priority_queue<Task, std::vector<Task>, TaskCompare> task_pq;
        task_pq.emplace(1, "Low priority");
        task_pq.emplace(10, "Urgent");
        task_pq.emplace(5, "Medium");
        assert(task_pq.top().name == "Urgent");
        std::cout << "  - Emplace with Struct: Passed\n";
    }

    // 5. Test Move Semantics
    {
        my_priority_queue<int> pq1;
        pq1.push(100);
        my_priority_queue<int> pq2 = std::move(pq1);
        assert(pq2.top() == 100);
        assert(pq1.empty()); // Source should be empty after move
        std::cout << "  - Move Semantics: Passed\n";
    }

    // 6. Test Swap
    {
        my_priority_queue<int> a, b;
        a.push(1);
        b.push(99);
        a.swap(b);
        assert(a.top() == 99);
        assert(b.top() == 1);
        std::cout << "  - Swap: Passed\n";
    }

    // 7. Test Assignment Operators
    {
        my_priority_queue<int> pq1;
        pq1.push(50);
        my_priority_queue<int> pq2;
        pq2 = pq1; // Copy assignment
        assert(pq2.top() == 50);
        
        my_priority_queue<int> pq3;
        pq3 = std::move(pq2); // Move assignment
        assert(pq3.top() == 50);
        std::cout << "  - Assignment Operators: Passed\n";
    }

    std::cout << "All tests passed successfully!\n";
}

int main() {
    std::cout << "\tStack testing\n";
    my_stack<int> st;

    std::cout << "pushing 3, 4\n";
    st.push(3);
    st.push(4);

    std::cout << "call pop(): " <<  st.pop() << '\n';

    std::cout << "call pop(): " <<  st.pop() << '\n';


    //std::cout << "call pop() for now exception working:\n";
    //st.pop();


    std::cout << "size of stack: " << st.size() << "\nis empty stack? : " <<  st.empty() << std::endl;

    //Queue
    std::cout << "\n\tQueue testing\n";
    my_queue<std::string> read;

    std::cout << "pushing \"World\" and \"Hello\" and \"brats\"\n";
    read.push("World");
    read.push("Hello");
    read.push("brats");

    std::cout << "call pop() : delete World\n";
    read.pop();

    std::cout << "calling front(): " << read.front() << '\n';
    std::cout << "calling back(): " << read.back() << '\n';

    std::cout << "size: " << read.size() << ", is queue empty? : " << read.empty() << std::endl;
    

    std::cout << "\n\tpriority_queue testing\n";
    run_tests();



    return 0;
}
