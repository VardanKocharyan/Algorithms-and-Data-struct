#include <iostream>
#include <cassert>
#include <string>

#include <mystl/sequence/deque.hpp>

using namespace mystl;

struct Person {
    std::string name;
    int age;

    Person(std::string n, int a) : name(std::move(n)), age(a) {
        std::cout << "  [Constructed] " << name << " (" << age << ")\n";
    }

    Person(const Person& other) : name(other.name), age(other.age) {
        std::cout << "  [Copy Constructed] " << name << "\n";
    }

    Person(Person&& other) noexcept : name(std::move(other.name)), age(other.age) {
        std::cout << "  [Move Constructed] " << name << "\n";
    }

    ~Person() = default;
};

void test_push_and_access() {
    std::cout << "=== 1. Test Push Back/Front & Access ===\n";
    deque<int> dq;

    dq.push_back(10);
    dq.push_back(20);
    dq.push_front(5);
    dq.push_front(1); // Deque layout: [1, 5, 10, 20]

    assert(dq.size() == 4);
    assert(dq.front() == 1);
    assert(dq.back() == 20);

    // Operator[] access
    assert(dq[0] == 1);
    assert(dq[1] == 5);
    assert(dq[2] == 10);
    assert(dq[3] == 20);

    std::cout << "Elements in dq: ";
    for (size_t i = 0; i < dq.size(); ++i) {
        std::cout << dq[i] << " ";
    }
    std::cout << "\nPASSED!\n\n";
}

void test_emplace() {
    std::cout << "=== 2. Test Emplace Back/Front (In-Place Construction) ===\n";
    deque<Person> people;

    std::cout << "1. Emplacing 'Alice' to back...\n";
    people.emplace_back("Alice", 25);

    std::cout << "2. Emplacing 'Bob' to front...\n";
    people.emplace_front("Bob", 30); 

    assert(people.size() == 2);
    assert(people.front().name == "Bob");
    assert(people.back().name == "Alice");

    std::cout << "PASSED!\n\n";
}

void test_iterators_and_realloc() {
    std::cout << "=== 3. Test Iterators & Large Allocation (500 Elements) ===\n";
    deque<int> dq;

    for (int i = 0; i < 500; ++i) {
        dq.push_back(i);
    }

    assert(dq.size() == 500);

    int expected = 0;
    for (int val : dq) {
        assert(val == expected);
        ++expected;
    }

    auto it = dq.begin();
    assert(*it == 0);

    it += 250; 
    assert(*it == 250);

    it -= 100;
    assert(*it == 150);

    std::cout << "PASSED!\n\n";
}

void test_pop() {
    std::cout << "=== 4. Test Pop Back & Pop Front ===\n";
    deque<int> dq;

    for (int i = 1; i <= 5; ++i) {
        dq.push_back(i); // [1, 2, 3, 4, 5]
    }

    dq.pop_back(); 
    assert(dq.back() == 4);

    dq.pop_front();
    assert(dq.front() == 2);

    assert(dq.size() == 3); 
    std::cout << "PASSED!\n\n";
}

void test_clear() {
    std::cout << "=== 5. Test Clear & Empty ===\n";
    deque<int> dq;

    for (int i = 0; i < 100; ++i) {
        dq.push_back(i);
    }

    assert(!dq.empty());
    dq.clear();
    assert(dq.empty());
    assert(dq.size() == 0);

    std::cout << "PASSED!\n\n";
}

int main() {
    try {
        test_push_and_access();
        test_emplace();
        test_iterators_and_realloc();
        test_pop();
        test_clear();

        std::cout << "========================================\n";
        std::cout << "🎉 ALL DEQUE TESTS PASSED SUCCESSFULLY! 🎉\n";
        std::cout << "========================================\n";
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
