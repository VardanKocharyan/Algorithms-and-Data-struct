#pragma once 
#include <vector>

template <typename T>
void bubbleSort(std::vector<T>& arr) {
    size_t n = arr.size();
    for (size_t i{}; i < n; ++i) {
       bool sw = false;
       for (size_t j{}; j < n - i - 1; ++j) {
           if (arr[j] > arr[j + 1]) {
               std::swap(arr[j], arr[j + 1]);
               sw = true;
           }
       }
       if (!sw) break;
    }
}
