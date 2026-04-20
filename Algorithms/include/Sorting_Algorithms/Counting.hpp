#pragma once
#include <vector>

void countingSort(std::vector<int>& nums) {
    int min{};
    int max{};

    if (!nums.empty()) {
        min = nums[0];
        max = nums[0];
    
    }

    for (size_t i{1}; i < nums.size(); ++i) {
        if (nums[i] < min) min = nums[i];
        if (nums[i] > max) max = nums[i];
    }

    int shift = -min;
    size_t range = max - min + 1;

    std::vector<int> frecuanceMap(range, 0);

    for (size_t i{}; i < nums.size(); ++i) {
        ++frecuanceMap[nums[i] + shift];
    }

    for (size_t i{}, j{}; i < range; ) {

        if (frecuanceMap[i]) {

            nums[j] = i - shift;

            ++j;
            --frecuanceMap[i];

        } else {
            ++i;
        }
    }
}
