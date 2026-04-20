#pragma once 
#include <vector>

int Median_of_Three(std::vector<int>& vec, int l, int r) {
    int m = l + (r - l) / 2;

    if ((vec[l] <= vec[m] && vec[m] <= vec[r]) || (vec[r] <= vec[m] && vec[m] <= vec[l])) return m;
    else if ((vec[m] <= vec[l] && vec[l] <= vec[r]) || (vec[r] <= vec[l] && vec[l] <= vec[m])) return l;

    return r;
}


int patrition_Median(std::vector<int>& vec, int l, int r) {
    int median = Median_of_Three(vec, l, r);

    std::swap(vec[median], vec[r]);
    
    int i = l - 1;

    for (int j = l; j <= r; ++j) {
        if (vec[j] <= vec[r]) {
            std::swap(vec[j], vec[++i]);
        }
    }
    return i;
}

void quickSort_Median(std::vector<int>& vec, int l, int r) {
    if (l < r) {
        int pivod = patrition_Median(vec, l, r);

        quickSort_Median(vec, l, pivod - 1);
        quickSort_Median(vec, pivod + 1, r);
    }
}
