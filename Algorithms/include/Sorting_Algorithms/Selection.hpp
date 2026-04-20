#pragma once 
#include <vector>

template <typename T>
void selectionSort(std::vector<T>& arr) {
    size_t n = arr.size();
    for (size_t i{}; i < n - 1; ++i) {
       for (size_t j = i + 1; j < n; ++j) {
           if (arr[i] > arr[j]) {
               std::swap(arr[i], arr[j]);
           }
       }
    }
}
