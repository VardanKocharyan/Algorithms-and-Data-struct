#pragma once 
#include <vector>

int partition_FirstEelem(std::vector<int>& vec, int l, int r) {
    int i = l;
    
    for (int j{l + 1}; j <= r; ++j) {
        if (vec[l] >= vec[j]) {
            std::swap(vec[j], vec[++i]);
        }
    }

    std::swap(vec[l], vec[i]);

    return i;
}

void quickSort_FirstElem(std::vector<int>& vec, int l, int r) {
    if (l < r) {
        int pivod = partition_FirstEelem(vec, l, r);

        quickSort_FirstElem(vec, l, pivod - 1);
        quickSort_FirstElem(vec, pivod + 1, r);
    }
}
