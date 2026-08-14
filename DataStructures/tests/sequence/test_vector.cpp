#include <iostream>
#include <string>
#include <cassert>
#include <mystl/sequence/vector.hpp>

struct Tracker {
    std::string name;
    Tracker(const std::string& n) : name(n) {
        std::cout << "  Constructed: " << name << "\n";
    }
    Tracker(const Tracker& other) : name(other.name + " (copy)") {
        std::cout << "  Copied: " << name << "\n";
    }
    Tracker(Tracker&& other) noexcept : name(std::move(other.name)) {
        other.name = "moved-from";
        std::cout << "  Moved: " << name << "\n";
    }
    ~Tracker() {
        if (!name.empty()) {
            std::cout << "  Destroyed: " << name << "\n";
        }
    }
};

int main() {
    std::cout << "=== Test 1: Push & Emplace ===\n";
    {
        mystl::vector<Tracker> vec;
        vec.reserve(2);
        vec.emplace_back("Element 1");
        vec.push_back(Tracker("Element 2"));
    }

    std::cout << "\n=== Test 2: Self-Insertion (v.push_back(v[0])) ===\n";
    {
        mystl::vector<std::string> vec;
        vec.push_back("Hello");
        vec.push_back("World");
        
        // Ստիպում ենք reallocation անել self-insertion-ի պահին
        vec.push_back(vec[0]); 

        assert(vec[2] == "Hello");
        std::cout << "Self-insertion successful: vec[2] = " << vec[2] << "\n";
    }

    std::cout << "\n=== Test 3: Iterators & Range-based For Loop ===\n";
    {
        mystl::vector<int> nums;
        for (int i = 1; i <= 5; ++i) {
            nums.push_back(i * 10);
        }

        std::cout << "Elements: ";
        for (const auto& val : nums) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n=== Test 4: Insert, Erase, Shrink to Fit ===\n";
    {
        mystl::vector<int> nums = {10, 20, 40}; // Եթե initializer_list constructor-ն ունեք
        nums.insert(nums.begin() + 2, 30); // 10, 20, 30, 40
        
        nums.erase(nums.begin() + 1); // 10, 30, 40

        std::cout << "After Insert & Erase: ";
        for (auto it = nums.begin(); it != nums.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";

        std::cout << "Capacity before shrink: " << nums.capacity() << "\n";
        nums.shrink_to_fit();
        std::cout << "Capacity after shrink: " << nums.capacity() << "\n";
    }

    std::cout << "\nALL TESTS PASSED SUCCESSFULLY!\n";
    return 0;
}
