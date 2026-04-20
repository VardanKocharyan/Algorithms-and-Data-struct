#pragma once 
#include <vector>

void merge(std::vector<int>& nums, int l, int m, int r) {
    
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<int> L(n1);
    std::vector<int> R(n2);

    for (int i{}; i < n1; ++i) {
        L[i] = nums[l + i];
    }
    for (int i{}; i < n2; ++i) {
        R[i] = nums[m + 1 + i];
    }

    int idx1{};
    int idx2{};
    while (idx1 < n1 && idx2 < n2) {
        if (L[idx1] <= R[idx2]) {
            nums[l] = L[idx1];
            ++idx1;
        } else {
            nums[l] = R[idx2];
            ++idx2;
        }
        ++l;
    }
    
    while (idx1 < n1) {
        nums[l] = L[idx1];
        ++idx1;
        ++l;
    }

    while (idx2 < n2) {
        nums[l] = R[idx2];
        ++idx2;
        ++l;
    }
}

void merge_sort(std::vector<int>& nums, int l, int r) {
    int m = l + (r - l) / 2;
    if(r <= l) return;
    merge_sort(nums, l, m);
    merge_sort(nums, m + 1, r);
    
    merge(nums, l, m, r);
}

void merge_sort(std::vector<int>& nums) {
    merge_sort(nums, 0, nums.size() - 1);
}
