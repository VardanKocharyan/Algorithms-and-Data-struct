#include <gtest/gtest.h>
#include <string>
#include <mystl/unordered/unordered_set.hpp>

TEST(UnorderedSetTest, InsertAndAccess) {
    mystl::unordered_set<int> set;
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.size(), 0);

    auto [it1, inserted1] = set.insert(10);
    EXPECT_TRUE(inserted1);
    EXPECT_EQ(*it1, 10);
    EXPECT_EQ(set.size(), 1);
    EXPECT_FALSE(set.empty());

    auto [it2, inserted2] = set.insert(20);
    EXPECT_TRUE(inserted2);
    EXPECT_EQ(*it2, 20);
    EXPECT_EQ(set.size(), 2);
}

TEST(UnorderedSetTest, EmplaceAndDuplicates) {
    mystl::unordered_set<std::string> set;

    auto [it1, inserted1] = set.emplace("hello");
    EXPECT_TRUE(inserted1);
    EXPECT_EQ(*it1, "hello");

    // Կրկնվող տարրի ավելացման փորձ
    auto [it2, inserted2] = set.emplace("hello");
    EXPECT_FALSE(inserted2);
    EXPECT_EQ(*it2, "hello");
    EXPECT_EQ(set.size(), 1);
}

TEST(UnorderedSetTest, ContainsAndFind) {
    mystl::unordered_set<int> set = {1, 2, 3, 4, 5};

    EXPECT_TRUE(set.contains(3));
    EXPECT_FALSE(set.contains(10));

    auto it = set.find(4);
    ASSERT_NE(it, set.end());
    EXPECT_EQ(*it, 4);

    auto it_not_found = set.find(100);
    EXPECT_EQ(it_not_found, set.end());
}

TEST(UnorderedSetTest, EraseOperations) {
    mystl::unordered_set<int> set = {10, 20, 30, 40};

    // Հեռացում ըստ արժեքի
    EXPECT_EQ(set.erase(20), 1);
    EXPECT_EQ(set.size(), 3);
    EXPECT_FALSE(set.contains(20));

    // Գոյություն չունեցող տարրի հեռացում
    EXPECT_EQ(set.erase(99), 0);
    EXPECT_EQ(set.size(), 3);

    // Հեռացում ըստ իտերատորի
    auto it = set.find(30);
    ASSERT_NE(it, set.end());
    set.erase(it);
    EXPECT_FALSE(set.contains(30));
    EXPECT_EQ(set.size(), 2);
}

TEST(UnorderedSetTest, RehashAndReserve) {
    mystl::unordered_set<int> set;
    set.reserve(100);
    EXPECT_GE(set.bucket_count(), 100);

    for (int i = 0; i < 50; ++i) {
        set.insert(i);
    }
    EXPECT_EQ(set.size(), 50);

    for (int i = 0; i < 50; ++i) {
        EXPECT_TRUE(set.contains(i));
    }
}

TEST(UnorderedSetTest, CopyAndMoveSemantics) {
    mystl::unordered_set<int> original = {1, 2, 3};

    // Copy constructor
    mystl::unordered_set<int> copy_constructed(original);
    EXPECT_EQ(copy_constructed.size(), 3);
    EXPECT_TRUE(copy_constructed.contains(1));

    // Copy assignment
    mystl::unordered_set<int> copy_assigned;
    copy_assigned = original;
    EXPECT_EQ(copy_assigned.size(), 3);
    EXPECT_TRUE(copy_assigned.contains(2));

    // Move constructor
    mystl::unordered_set<int> move_constructed(std::move(copy_constructed));
    EXPECT_EQ(move_constructed.size(), 3);
    EXPECT_TRUE(move_constructed.contains(3));

    // Move assignment
    mystl::unordered_set<int> move_assigned;
    move_assigned = std::move(copy_assigned);
    EXPECT_EQ(move_assigned.size(), 3);
    EXPECT_TRUE(move_assigned.contains(1));
}

TEST(UnorderedSetTest, Clear) {
    mystl::unordered_set<int> set = {1, 2, 3, 4, 5};
    EXPECT_EQ(set.size(), 5);

    set.clear();
    EXPECT_EQ(set.size(), 0);
    EXPECT_TRUE(set.empty());
    EXPECT_FALSE(set.contains(1));
}
