#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <mystl/unordered/unordered_map.hpp>

// 1. Core Insertion & Lookup Tests
TEST(UnorderedMapTest, InsertAndAccess) {
    mystl::unordered_map<std::string, int> map;

    EXPECT_TRUE(map.empty());
    EXPECT_EQ(map.size(), 0);

    // Operator[] insertion
    map["apple"] = 5;
    map["banana"] = 10;

    EXPECT_FALSE(map.empty());
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(map["apple"], 5);
    EXPECT_EQ(map["banana"], 10);

    // Overwriting existing key
    map["apple"] = 42;
    EXPECT_EQ(map["apple"], 42);
    EXPECT_EQ(map.size(), 2);
}

// 2. Emplace & Duplicate Prevention
TEST(UnorderedMapTest, EmplaceAndDuplicates) {
    mystl::unordered_map<int, std::string> map;

    auto [it1, inserted1] = map.emplace(1, "one");
    EXPECT_TRUE(inserted1);
    EXPECT_EQ(it1->first, 1);
    EXPECT_EQ(it1->second, "one");

    // Duplicate key insertion should fail and return iterator to existing element
    auto [it2, inserted2] = map.emplace(1, "ONE_DUPLICATE");
    EXPECT_FALSE(inserted2);
    EXPECT_EQ(it2->first, 1);
    EXPECT_EQ(it2->second, "one"); // Value shouldn't change
    EXPECT_EQ(map.size(), 1);
}

// 3. Exception Safety with at()
TEST(UnorderedMapTest, AtExceptionHandling) {
    mystl::unordered_map<std::string, int> map = {{"key1", 100}};

    EXPECT_EQ(map.at("key1"), 100);
    EXPECT_THROW(map.at("non_existing_key"), std::out_of_range);
}

// 4. Find & Iterator Traversal
TEST(UnorderedMapTest, FindAndIteration) {
    mystl::unordered_map<int, int> map;
    for (int i = 0; i < 100; ++i) {
        map[i] = i * 10;
    }

    EXPECT_EQ(map.size(), 100);

    // Find existing
    auto it = map.find(42);
    ASSERT_NE(it, map.end());
    EXPECT_EQ(it->second, 420);

    // Find non-existing
    EXPECT_EQ(map.find(999), map.end());

    // Iteration over container
    size_t count = 0;
    for (const auto& [key, value] : map) {
        EXPECT_EQ(value, key * 10);
        ++count;
    }
    EXPECT_EQ(count, 100);
}

// 5. Erase Logic Tests
TEST(UnorderedMapTest, EraseOperations) {
    mystl::unordered_map<int, std::string> map = {
        {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}
    };

    // Erase by key
    EXPECT_EQ(map.erase(2), 1);
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map.find(2), map.end());

    // Erase non-existing key
    EXPECT_EQ(map.erase(99), 0);
    EXPECT_EQ(map.size(), 3);

    // Erase by iterator
    auto it = map.find(3);
    ASSERT_NE(it, map.end());
    auto next_it = map.erase(it);
    
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(map.find(3), map.end());
    if (next_it != map.end()) {
        EXPECT_TRUE(next_it->first == 1 || next_it->first == 4);
    }
}

// 6. Rehashing and Load Factor Policy
TEST(UnorderedMapTest, RehashAndReserve) {
    mystl::unordered_map<int, int> map;
    
    // Force rehash via insertion of many elements
    size_t initial_bucket_count = map.bucket_count();
    for (int i = 0; i < 1000; ++i) {
        map[i] = i;
    }

    EXPECT_GT(map.bucket_count(), initial_bucket_count);
    EXPECT_LE(map.load_factor(), map.max_load_factor());

    // Verify all 1000 elements are preserved correctly after rehashing
    for (int i = 0; i < 1000; ++i) {
        EXPECT_EQ(map[i], i);
    }

    // Reserve test
    map.reserve(5000);
    EXPECT_GE(map.bucket_count(), 5000);
}

// 7. Copy and Move Semantics
TEST(UnorderedMapTest, CopyAndMoveSemantics) {
    mystl::unordered_map<int, std::string> original = {{1, "a"}, {2, "b"}};

    // Copy Constructor
    mystl::unordered_map<int, std::string> copy = original;
    EXPECT_EQ(copy.size(), 2);
    EXPECT_EQ(copy[1], "a");
    EXPECT_EQ(original.size(), 2); // Original intact

    // Move Constructor
    mystl::unordered_map<int, std::string> moved = std::move(copy);
    EXPECT_EQ(moved.size(), 2);
    EXPECT_EQ(moved[1], "a");
    EXPECT_TRUE(copy.empty()); // Moved-from state
}

// 8. Clear Operations
TEST(UnorderedMapTest, Clear) {
    mystl::unordered_map<int, int> map = {{1, 10}, {2, 20}, {3, 30}};
    EXPECT_EQ(map.size(), 3);

    map.clear();
    EXPECT_EQ(map.size(), 0);
    EXPECT_TRUE(map.empty());
    EXPECT_EQ(map.find(1), map.end());
}
