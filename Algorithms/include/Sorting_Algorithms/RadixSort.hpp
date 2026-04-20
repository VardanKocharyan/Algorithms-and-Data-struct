#pragma once
#include <vector>

void countingSort(std::vector<int>& nums, int exp) {

    int range = 10;
    std::vector<int> count(range, 0);

    for (auto n : nums) {
        ++count[(n / exp) % 10];
    }
//Prefix Sum
    for (int i{1}; i < range; ++i) {
        count[i] += count[i - 1];
    }

    std::vector<int> out(nums.size());

    for (int i = nums.size() - 1; i >= 0; --i) {
        out[--count[(nums[i] / exp) % 10]] = nums[i];
    }

    nums = std::move(out);
}

void radixSort(std::vector<int>& vec) {
    int max = vec[0];

    for (auto n : vec) 
        if (max < n) 
            max = n;

    for (int exp{1}; max / exp > 0; exp *= 10) {
        countingSort(vec, exp);
    }
}
