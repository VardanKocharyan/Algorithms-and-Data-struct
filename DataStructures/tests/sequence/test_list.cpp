#include <gtest/gtest.h>
#include "mystl/sequence/list.hpp"

#include <string>
#include <vector>
#include <numeric>

// Auxiliar struct for RAII / Destructor tracking
struct InstanceTracker {
    static inline int live_instances = 0;
    int value;

    explicit InstanceTracker(int v = 0) : value(v) {
        ++live_instances;
    }

    InstanceTracker(const InstanceTracker& other) : value(other.value) {
        ++live_instances;
    }

    InstanceTracker(InstanceTracker&& other) noexcept : value(other.value) {
        ++live_instances;
    }

    ~InstanceTracker() {
        --live_instances;
    }

    bool operator==(const InstanceTracker& other) const {
        return value == other.value;
    }
};

// -----------------------------------------------------------------------------
// Milestone 1 & 2: Constructors, Iterators, and Basic States
// -----------------------------------------------------------------------------

TEST(ListTest, DefaultConstructor) {
    mystl::list<int> l;
    EXPECT_EQ(l.size(), 0);
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.begin(), l.end());
}

TEST(ListTest, IteratorTraversalAndDereference) {
    mystl::list<int> l;
    l.push_back(10);
    l.push_back(20);
    l.push_back(30);

    auto it = l.begin();
    EXPECT_EQ(*it, 10);
    ++it;
    EXPECT_EQ(*it, 20);
    it++;
    EXPECT_EQ(*it, 30);
    --it;
    EXPECT_EQ(*it, 20);

    // Const iterator test
    const auto& const_l = l;
    int sum = 0;
    for (auto const_it = const_l.cbegin(); const_it != const_l.cend(); ++const_it) {
        sum += *const_it;
    }
    EXPECT_EQ(sum, 60);
}

// -----------------------------------------------------------------------------
// Milestone 3: Core Modifiers (push, pop, emplace, erase, clear)
// -----------------------------------------------------------------------------

TEST(ListTest, PushAndPopOperations) {
    mystl::list<std::string> l;

    l.push_back("world");
    l.push_front("hello");

    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(*l.begin(), "hello");

    l.pop_front();
    EXPECT_EQ(l.size(), 1);
    EXPECT_EQ(*l.begin(), "world");

    l.pop_back();
    EXPECT_EQ(l.size(), 0);
    EXPECT_TRUE(l.empty());
}

TEST(ListTest, EmplaceBackAndFront) {
    struct Person {
        std::string name;
        int age;
        Person(std::string n, int a) : name(std::move(n)), age(a) {}
    };

    mystl::list<Person> people;
    people.emplace_back("Alice", 30);
    people.emplace_front("Bob", 25);

    EXPECT_EQ(people.size(), 2);
    EXPECT_EQ(people.begin()->name, "Bob");
    EXPECT_EQ(people.begin()->age, 25);
}

TEST(ListTest, InsertAndEraseVariants) {
    mystl::list<int> l;
    l.push_back(1);
    l.push_back(4);

    // Insert single element
    auto it = l.begin();
    ++it; // Points to 4
    l.insert(it, 3); // list: [1, 3, 4]

    // Emplace
    it = l.begin();
    ++it; // Points to 3
    l.emplace(it, 2); // list: [1, 2, 3, 4]

    std::vector<int> expected = {1, 2, 3, 4};
    std::vector<int> actual(l.begin(), l.end());
    EXPECT_EQ(actual, expected);

    // Erase single element
    it = l.begin();
    ++it; // Points to 2
    it = l.erase(it); // Erases 2, returns iterator to 3
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(l.size(), 3);
}

// -----------------------------------------------------------------------------
// Milestone 4: Resource Management & RAII (Rule of 5, Exception Safety)
// -----------------------------------------------------------------------------

TEST(ListTest, RAIIAndDestructorCall) {
    InstanceTracker::live_instances = 0;
    {
        mystl::list<InstanceTracker> l;
        l.emplace_back(1);
        l.emplace_back(2);
        l.emplace_back(3);
        EXPECT_EQ(InstanceTracker::live_instances, 3);
        l.pop_back();
        EXPECT_EQ(InstanceTracker::live_instances, 2);
    }
    // After destruction of list, all instances must be freed
    EXPECT_EQ(InstanceTracker::live_instances, 0);
}

TEST(ListTest, CopyConstructorAndAssignment) {
    mystl::list<int> original;
    original.push_back(100);
    original.push_back(200);

    // Deep copy constructor
    mystl::list<int> copy_constructed = original;
    EXPECT_EQ(copy_constructed.size(), 2);
    EXPECT_EQ(*copy_constructed.begin(), 100);

    // Modify copy to ensure independence
    copy_constructed.push_back(300);
    EXPECT_EQ(original.size(), 2);
    EXPECT_EQ(copy_constructed.size(), 3);

    // Copy assignment
    mystl::list<int> copy_assigned;
    copy_assigned = original;
    EXPECT_EQ(copy_assigned.size(), 2);
    EXPECT_EQ(*copy_assigned.begin(), 100);
}

TEST(ListTest, MoveConstructorAndAssignment) {
    mystl::list<int> source;
    source.push_back(1);
    source.push_back(2);

    // Move constructor
    mystl::list<int> moved_to(std::move(source));
    EXPECT_EQ(moved_to.size(), 2);
    EXPECT_TRUE(source.empty());

    // Move assignment
    mystl::list<int> move_assigned;
    move_assigned = std::move(moved_to);
    EXPECT_EQ(move_assigned.size(), 2);
    EXPECT_TRUE(moved_to.empty());
}

// -----------------------------------------------------------------------------
// Milestone 5: Pointer Manipulation (Splice, Reverse, Remove, Merge)
// -----------------------------------------------------------------------------

TEST(ListTest, SpliceEntireList) {
    mystl::list<int> l1;
    l1.push_back(1);
    l1.push_back(2);

    mystl::list<int> l2;
    l2.push_back(3);
    l2.push_back(4);

    l1.splice(l1.end(), l2); // Append l2 to l1

    EXPECT_EQ(l1.size(), 4);
    EXPECT_TRUE(l2.empty());

    std::vector<int> expected = {1, 2, 3, 4};
    std::vector<int> actual(l1.begin(), l1.end());
    EXPECT_EQ(actual, expected);
}

TEST(ListTest, ReverseInPlace) {
    mystl::list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    l.reverse();

    std::vector<int> expected = {3, 2, 1};
    std::vector<int> actual(l.begin(), l.end());
    EXPECT_EQ(actual, expected);
}

TEST(ListTest, RemoveAndRemoveIf) {
    mystl::list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(2);
    l.push_back(4);

    l.remove(2); // Removes all '2's
    EXPECT_EQ(l.size(), 3);

    l.remove_if([](int x) { return x % 2 != 0; }); // Removes odds (1, 3)
    EXPECT_EQ(l.size(), 1);
    EXPECT_EQ(*l.begin(), 4);
}

TEST(ListTest, MergeSortedLists) {
    mystl::list<int> l1;
    l1.push_back(1);
    l1.push_back(3);
    l1.push_back(5);

    mystl::list<int> l2;
    l2.push_back(2);
    l2.push_back(4);
    l2.push_back(6);

    l1.merge(l2);

    EXPECT_EQ(l1.size(), 6);
    EXPECT_TRUE(l2.empty());

    std::vector<int> expected = {1, 2, 3, 4, 5, 6};
    std::vector<int> actual(l1.begin(), l1.end());
    EXPECT_EQ(actual, expected);
}
