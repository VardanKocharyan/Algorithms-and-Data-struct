#pragma once 
#include <vector>

int rec_bs(const std::vector<int>& nums, int l, int r, const int target) {
    if (l > r) return - 1;

    int m = l + (r - l) / 2;

    if (target < nums[m]) return rec_bs(nums, l, m - 1, target);
    if (target > nums[m]) return rec_bs(nums, m + 1, r, target);
    return m;
}


int rec_binarySearch(const std::vector<int>& nums, const int target) {
    return rec_bs(nums, 0, nums.size() - 1, target);
}
