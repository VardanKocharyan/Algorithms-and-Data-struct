#pragma once
#include <iostream>
#include <vector>

int binarySearch(const std::vector<int>& nums, const int val) {
    int l{0}, r = nums.size() - 1;

    while (l <= r) {
        int m = l + (r - l) / 2;
        if (nums[m] < val) l = m + 1;
        else if (nums[m] > val) r = m - 1;
        else return m;
    }
    return -1;
}
