#pragma once
#include <vector>

void countingSort_Stable(std::vector<int>& nums) {

    int min = nums[0];
    int max = min;

    for (auto n : nums) {
        if (n < min) min = n;
        if (n > max) max = n;
    }

    int range = max - min + 1;
    std::vector<int> count(range, 0);

    for (auto n : nums) {
        ++count[n - min];
    }
//Prefix Sum
    for (int i{1}; i < range; ++i) {
        count[i] += count[i - 1];
    }

    std::vector<int> out(nums.size());

    for (int i = nums.size() - 1; i >= 0; --i) {
        out[--count[nums[i] - min]] = nums[i];
    }

    nums = std::move(out);
}
