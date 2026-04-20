#pragma once 
#include <vector>

void bubbleItSort(std::vector<int>& nums) {

    auto next = nums.begin();
    auto end = nums.end();

    while (next != end) {
        bool sw = false;
        ++next; 

        while (next != end) {
            auto prev = next - 1;
            if (*prev > *next) {
                std::swap(*prev, *next);
                sw = true;
            }
            ++next;
        }
        if(!sw) break;

        next = nums.begin();
        --end;
    }
}
