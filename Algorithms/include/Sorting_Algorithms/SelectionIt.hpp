#pragma once
#include <vector>

template<typename It, typename Comp>
It myMin(It it, It end, Comp comp) {
    It min = it++;
    for ( ;it != end; ++it) {
        min = comp(*min, *it) ? min : it;
    }
    return min;
}

void selectionItSort(std::vector<int>& nums) {
    auto end = nums.end();
    auto it = nums.begin();

    while (it != end) {

        auto min = myMin(it, end, [](int a, int b) {
                return a < b;
                });
        
        if (min != it) {
            std::swap(*min, *it);
        }

        ++it;
    }
}
