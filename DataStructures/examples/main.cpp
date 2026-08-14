#include <iostream>
#include <mystl/sequence/vector.hpp>

int main() {
    mystl::vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);

    std::cout << "Vector size: " << vec.size() << std::endl;
    std::cout << "Vector element 0: " << vec[0] << std::endl;

    return 0;
}
