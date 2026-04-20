#pragma once
#include <vector>

void insertionSort(std::vector<int>& nums) {
    int n = nums.size();
    for (int i{1}; i < n; ++i) {
        int key = nums[i];
        int j = i - 1;

        while (j >= 0 && key < nums[j])
        {
            nums[j + 1] = nums[j];
            --j;
        }
        
        if (i != j + 1) nums[i] = key;
    }
}

