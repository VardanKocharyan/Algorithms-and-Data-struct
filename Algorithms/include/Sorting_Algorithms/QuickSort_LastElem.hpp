#pragma once 
#include <vector>

int partition_LastElem(std::vector<int>& vec, int l, int r) {
    int i = l - 1;

    for (int j{l}; j <= r; ++j) {
        if (vec[r] >= vec[j]) {
            std::swap(vec[j], vec[++i]);
        }
    }
    return i;
}

void quickSort_LastElem(std::vector<int>& vec, int l, int r) {
    if (l < r) {
        int pivod = partition_LastElem(vec, l, r);

        quickSort_LastElem(vec, l, pivod - 1);
        quickSort_LastElem(vec, pivod + 1, r);
    }
}
