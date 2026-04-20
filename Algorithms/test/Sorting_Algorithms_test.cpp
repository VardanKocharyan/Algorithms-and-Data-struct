#include <iostream>
#include <vector>
#include <x86intrin.h>
#include <cstdlib>
#include <ctime>
#include "SortingAlgorithms.hpp"

inline unsigned long long read_cycles() {
    return __rdtsc();
}

void Bubble_test(std::vector<int> arr) {
    unsigned long long start = read_cycles();
    bubbleSort(arr);
    unsigned long long end = read_cycles();
    
    std::cout << "Bubble execution: " << (end - start) << " CPU cycles\n";
}

void Selection_test(std::vector<int> arr) {
    unsigned long long start = read_cycles();
    selectionSort(arr);
    unsigned long long end = read_cycles();
    
    std::cout << "Selection execution: " << (end - start) << " CPU cycles\n";
}

void BubbleIt_test(std::vector<int> arr) {
    unsigned long long start = read_cycles();
    bubbleItSort(arr);
    unsigned long long end = read_cycles();
    
    std::cout << "Bubble Iterator execution: " << (end - start) << " CPU cycles\n";
}

void SelectionIt_test(std::vector<int> arr) {
    unsigned long long start = read_cycles();
    selectionItSort(arr);
    unsigned long long end = read_cycles();
    
    std::cout << "Selection Iterator execution: " << (end - start) << " CPU cycles\n";
}

void Counting_test(std::vector<int> arr) {
    unsigned long long start = read_cycles();
    countingSort(arr);
    unsigned long long end = read_cycles();
    
    std::cout << "Counting execution: " << (end - start) << " CPU cycles\n";
}

void Counting_Stable_test(std::vector<int> arr) {
    unsigned long long start = read_cycles();
    countingSort_Stable(arr);
    unsigned long long end = read_cycles();
    
    std::cout << "Counting Stable execution: " << (end - start) << " CPU cycles\n";
}

void Radix_Sort_test(std::vector<int> arr) {
    unsigned long long start = read_cycles();
    radixSort(arr);
    unsigned long long end = read_cycles();
    
    std::cout << "Radix Sort execution: " << (end - start) << " CPU cycles\n";
}

void Insertion_test(std::vector<int> arr) {
    unsigned long long start = read_cycles();
    insertionSort(arr);
    unsigned long long end = read_cycles();
    
    std::cout << "Insertion execution: " << (end - start) << " CPU cycles\n";
}

void Merge_Sort_test(std::vector<int> arr) {
    unsigned long long start = read_cycles();
    merge_sort(arr);
    unsigned long long end = read_cycles();

    std::cout << "Merge execution: " << (end - start) << " CPU cycles\n";
}

void Quick_Sort_Last_Elem_test(std::vector<int> arr) {
    unsigned long long start = read_cycles();
    quickSort_LastElem(arr, 0, arr.size() - 1);
    unsigned long long end = read_cycles();

    std::cout << "Quick pivod = Last elem. execution: " << (end - start) << " CPU cycles\n";
}

void Quick_Sort_First_Elem_test(std::vector<int> arr) {
    unsigned long long start = read_cycles();
    quickSort_FirstElem(arr, 0, arr.size() - 1);
    unsigned long long end = read_cycles();

    std::cout << "Quick pivod = First elem. execution: " << (end - start) << " CPU cycles\n";
}

void Quick_Sort_Median_Elem_test(std::vector<int> arr) {
    unsigned long long start = read_cycles();
    quickSort_Median(arr, 0, arr.size() - 1);
    unsigned long long end = read_cycles();

    std::cout << "Quick pivod = Median elem. execution: " << (end - start) << " CPU cycles\n";
}

void Quick_Sort_Rand_Elem_test(std::vector<int> arr) {
    unsigned long long start = read_cycles();
    quickSort_Rand(arr, 0, arr.size() - 1);
    unsigned long long end = read_cycles();

    std::cout << "Quick pivod = Random elem. execution: " << (end - start) << " CPU cycles\n";
}

int BinarySearch_test(const std::vector<int>& arr, const int val) {
    std::cout << "BinarySearch : ";
    return binarySearch(arr, val); 
}

int Rec_BinarySearch_test(const std::vector<int>& arr, const int val) {
    std::cout << "Recursive BinarySearch : ";
    return rec_binarySearch(arr, val); 
}



int main() {
    std::srand(std::time(0));

    std::vector<int> v{77, 3, 2, 0, 711, 9, 57, 9, 12, 1, 111};

    std::cout << "\tComparing algorithms for N = " << v.size() << " elements:\n";

    Bubble_test(v);
    BubbleIt_test(v);

    Selection_test(v);
    SelectionIt_test(v);

    //Counting_test(v);
    //Counting_Stable_test(v);
    //Radix_Sort_test(v); // only for > 0 array numbers
    
    Insertion_test(v);
    
    Merge_Sort_test(v);
    
    Quick_Sort_Last_Elem_test(v);
    Quick_Sort_First_Elem_test(v);
    Quick_Sort_Median_Elem_test(v);
    Quick_Sort_Rand_Elem_test(v);

/*
    std::vector<int> binarySearch{1, 2, 3, 4, 5, 6, 7, 9, 9, 10};

    std::cout << "\n\tBinary Search N = " << binarySearch.size() << " elements:\n";

    std::cout << BinarySearch_test(binarySearch, 0) << '\n';
    std::cout << Rec_BinarySearch_test(binarySearch, 7) << '\n';

    std::cout << "\tLower and Upper bound\n";

    int lbound = lu::lower_bound(binarySearch, 7);
    std::cout << "Lower_bound: " << binarySearch[lbound] << '\n';

    int ubound = lu::upper_bound(binarySearch, 7);
    std::cout << "Upper bound: " << binarySearch[ubound] << '\n';
*/
    return 0;
}
