#pragma once

namespace lu {

int lower_bound(const std::vector<int>& nums, const int& target) {
    
    int left{}, right = nums.size();

    while (left < right) {
        int m = left + (right - left) / 2;
        if (nums[m] < target) {
            left = m + 1;
        } else {
            right = m;
        }
    }
    return left;
}

int upper_bound(const std::vector<int>& nums, const int& target) {

    int left{}, right = nums.size();

    while (left < right) {
        int m = left + (right - left) / 2;
        if (nums[m] <= target) {
            left = m + 1;
        } else {
            right = m;
        }
    }
    return left;   
}

}
