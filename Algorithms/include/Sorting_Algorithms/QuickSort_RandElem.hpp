#pragma once 
#include <vector>
#include <cstdlib>

int Median_of_Three_Rand(std::vector<int>& vec, int l, int r) {
    int idx1 = l + std::rand() % (r - l + 1);
    int idx2 = l + std::rand() % (r - l + 1);
    int idx3 = l + std::rand() % (r - l + 1);

    if ((vec[idx1] <= vec[idx2] && vec[idx2] <= vec[idx3]) || (vec[idx3] <= vec[idx2] && vec[idx2] <= vec[idx1])) return idx2;
    else if ((vec[idx2] <= vec[idx1] && vec[idx1] <= vec[idx3]) || (vec[idx3] <= vec[idx1] && vec[idx1] <= vec[idx2])) return idx1;

    return idx3;
}

int patrition_Rand(std::vector<int>& vec, int l, int r) {
    int median = Median_of_Three_Rand(vec, l, r);

    std::swap(vec[median], vec[r]);
    
    int i = l - 1;

    for (int j = l; j <= r; ++j) {
        if (vec[j] <= vec[r]) {
            std::swap(vec[j], vec[++i]);
        }
    }
    return i;
}

void quickSort_Rand(std::vector<int>& vec, int l, int r) {
    if (l < r) {
        int pivod = patrition_Rand(vec, l, r);

        quickSort_Rand(vec, l, pivod - 1);
        quickSort_Rand(vec, pivod + 1, r);
    }
}
